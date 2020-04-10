
#include "Arduino.h"
#include "RFBee_Interface.h"

RFBee rfbee;

char dataToSend[] = "Alles schick in der Physik";

void setup(){
  //start the sensor enviroment
  rfbee.Startup();
}

void loop()
{
  if(rfbee.Update())
  {
    char message[63] = {0};
    rfbee.getMessage(message);
    Serial.write(message);
    rfbee.sendData(dataToSend);
  }
}
