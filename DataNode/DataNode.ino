#include "Arduino.h"
#include "Sensor.h"
#include "RFBee_Interface.h"

//global variables
#define DATA_PIN 5 //see http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info ( maybe it's 6)

//class creation
TH_Sensor th_sensor(DATA_PIN); //creates the object to talk to the Sensor
RFBee rfbee;                   //creates the object to talk to the RFBee

void setup() 
{
    Serial.begin(9600); // sets communication speed
    rfbee.Startup();    //startup 
    th_sensor.Startup();  //starts the sensor-related class   
}

void loop() {
  if(rfbee.Update())
  {
    char message[63] = {0};
    rfbee.getMessage(message);
    Serial.write(message);
    Serial.print("\n");
    float temp_humid[2] = {0};
    if(th_sensor.get_temp_humid(temp_humid))
    {
      char dataToSend[63];
      char temp[10];
      char string_1[] = "Temperature: ";
      char string_2[] = "\nHumidity: ";
      rfbee.sendData(string_1, sizeof(string_1)/sizeof(char));
      dtostrf(temp_humid[0], 4, 2, temp);
      rfbee.sendData(temp, 10);
      for(uint8_t i=0; i<10;i++)
      {
        temp[i]=0;
      }
      rfbee.sendData(string_2, sizeof(string_2)/sizeof(char));
      dtostrf(temp_humid[1], 4, 2, temp);
      rfbee.sendData(temp, 10);
    }
    else
    {
      char string[] = "Couldn't get any data";
      rfbee.sendData(string, sizeof(string)/sizeof(char));   //send the temperature values
    }
    
  }
}
