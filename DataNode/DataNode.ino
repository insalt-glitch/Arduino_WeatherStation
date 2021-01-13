/*
 Arduino.h - Main include file for the Arduino SDK
 Copyright (c) 2005-2013 Arduino Team.  All right reserved.
 */
#include "Arduino.h"
#include "Sensor.h"
#include "RFBee_Interface.h"

// global variables
// http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info
#define DATA_PIN 5

// class creation
// creates the object to talk to the Sensor
TH_Sensor th_sensor(DATA_PIN);
// creates the object to talk to the RFBee
RFBee rfbee;

void setup()
{
    // sets communication speed
    Serial.begin(9600);
    // startup
    rfbee.Startup();
    // starts the sensor-related class
    th_sensor.Startup();
}

void loop() {
  if (rfbee.Update()) {
    char message[63] = {0};
    // TODO(Nils): We shouldn't have to recieve any data on the node except
    // for setting-changes which should maybe rather be done manualy?
    rfbee.getMessage(message);
    Serial.write(message);
    Serial.print("\n");
    float temp_humid[2] = {0};
    if (th_sensor.get_temp_humid(temp_humid)) {
      // TODO(Nils): Check if we should init this to something
      // We send the data with a default width of data_width
      int data_width = 10;
      char temp[data_width];
      // TODO (Nils): We should probably just send the humidty and temperature.
      char string_1[] = "Temperature: ";
      char string_2[] = "\nHumidity: ";
      rfbee.sendData(string_1, sizeof(string_1)/sizeof(char));
      // converts the float to a string and stores it in temp
      // char* dtostrf(double __val, signed char __width, unsigned char __prec, char* __s)
      dtostrf(temp_humid[0], 4, 2, temp);
      rfbee.sendData(temp, data_width);
      // clear the temp array
      for (uint8_t i = 0; i < data_width; i++) {
        temp[i] = 0;
      }
      rfbee.sendData(string_2, sizeof(string_2)/sizeof(char));
      // converts the float to a string and stores it in temp
      dtostrf(temp_humid[1], 4, 2, temp);
      rfbee.sendData(temp, data_width);
    }
    else {
      char string[] = "Couldn't get any data";
      // send the temperature values
      rfbee.sendData(string, sizeof(string)/sizeof(char));
    }
  }
}
