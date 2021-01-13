#include "Arduino.h"
#include "RFBee_Interface.h"

RFBee rfbee;

char dataToSend[] = "Alles schick in der Physik";
long unsigned int lastSend = 0;

void setup(){
  //start the sensor enviroment
  Serial.begin(9600);
  rfbee.Startup();
  //Serial.println();
}

void loop()
{
  if(rfbee.Update())
  {
    char message[63] = {0};
    rfbee.getMessage(message);
    Serial.write(message);
    Serial.print("\n");
    //rfbee.sendData(dataToSend, sizeof(dataToSend)/sizeof(char));
  }
  if((millis()-lastSend)>10000)
  {
    char c[] = "hello";
    rfbee.sendData(c, sizeof(c)/sizeof(char));
    lastSend=millis();
  }

}
