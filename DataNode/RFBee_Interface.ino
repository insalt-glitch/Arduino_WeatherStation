#include "RFBee_Interface.h"
#include "debug.h"
#include "globals.h"
#include "Config.h"
#include "CCx.h"
#include "rfBeeSerial.h"

void RFBee::sendData(char* data, uint8_t len)
{
    //TODO(Nils): this is terrible stuff
    sendOwnData(data, len);
}

void RFBee::getMessage(char* message)
{
  for(int i=0; i<63;i++)
  {
    message[i] = messageRecieved[i];
    messageRecieved[i]=0;
  }
}

boolean RFBee::Update(void)
{
  char dataIn[CCx_PACKT_LEN] = {0};
  rfSensorUpdate(dataIn);
  //TODO(Nils): I feel like the implementation leaves much to be desired
  //First package of the message or the end of the message
  if ((millis()-firstPackageRecieved) > 500)
  {
    // We didn't recieve a package, so we consider this end of message and send it to the user
    if((messageRecieved[0] != 0) && (dataIn[0] == 0))
    {
      recievedBits = 0;
      return true;
    }
    // This is the first package of the message, so we write that down
    else if (dataIn[0] != 0)
    {
      firstPackageRecieved = millis();
    }
  }
  //We recieved a message
  if(dataIn[0] != 0)
  {
    int i = 0;
    while(dataIn[i])
    {
      messageRecieved[recievedBits + i] = dataIn[i];
      i++;
    }
    recievedBits += i;
  }
  return false;
}

void RFBee::Startup(void)
{
  pinMode(9,OUTPUT);
  if (Config.initialized() != OK) {
    Config.reset();
  }
  Config.load_default();
  //do extra initalization
  myAddress = logAddress;
  Config.set(CONFIG_MY_ADDR,myAddress);//modify the numberf to specify an unique address for RFBee itself
  setMyAddress();
  Config.set(CONFIG_ADDR_CHECK,2);//2:set RFBee with address checking and broadcast
  setAddressCheck();
  Config.set(CONFIG_OUTPUT_FORMAT,1); // 0: only payload , 1: srcAddr dstAddr payload
  Config.set(CONFIG_BDINDEX,0);//BaudRate (0): 9600, (2): 38400 bps
  setUartBaudRate();
  rfBeeInit();
}

void RFBee::rfSensorUpdate(char* dataOutput) //main loop
{
    byte mode = Config.get(CONFIG_RFBEE_MODE);
  if (Serial.available() > 0)
  {
    sleepCounter=1000; // reset the sleep counter
    if (serialMode == SERIALCMDMODE)
    {
      readSerialCmd();
      return;
    }
    else
    {
      txDataFromSerialToRf();
    }
  }
  if (mode != TRANSMIT_MODE) {
    if (digitalRead(GDO0) == HIGH) {
      sleepCounter=1000; // reset the sleep counter
      //rxDataFromRfToSerial();
      rftoSerial(dataOutput);
      sleepCounter++; // delay sleep
    }
    else if (millis() - CCx.lastCalibrate > 60000) { //re-calibrate cc when its idle after 1min
      CCx.Strobe(CCx_SIDLE);
      CCx.Strobe(CCx_SRX);
      CCx.lastCalibrate = millis();
    }
  }
  sleepCounter--;
  // check if we can go to sleep again, going into low power too early will result in lost data in the CCx fifo.
  if ((sleepCounter == 0) && (mode == LOWPOWER_MODE)) {
//    DEBUGPRINT("low power on")
    lowPowerOn();
//    DEBUGPRINT("woke up")
  }
}

void RFBee::rfBeeInit(void){
//    DEBUGPRINT()
    CCx.PowerOnStartUp();
    setCCxConfig();
    serialMode=SERIALDATAMODE;
    sleepCounter=0;
    attachInterrupt(0, ISRVreceiveData, RISING);  //GD00 is located on pin 2, which results in INT 0
    pinMode(GDO0,INPUT);// used for polling the RF received data
}

// handle interrupt
void ISRVreceiveData(){
//  DEBUGPRINT()
  sleepCounter=10;
}

void RFBee::rftoSerial(byte* dataOutput){
  byte rxData[CCx_PACKT_LEN];
  byte len;
  byte srcAddress;
  byte destAddress;
  char rssi;
  byte lqi;
  int result;
  result = receiveData(dataOutput, &len, &srcAddress, &destAddress, (byte *)&rssi, &lqi);
  if (result == ERR || result == NOTHING) return;
  //Serial.print(srcAddress,DEC);
  delay(100);
  return;
}
