#include "Arduino.h"
#include "Sensor.h"

//global variables
const int DATA_PIN = 5; //see http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info ( maybe it's 6)

//class creation
TH_Sensor th_sensor(DATA_PIN); //creates the object to talk to the sensor

void setup()
{
    Serial.begin(9600); // sets communication speed
    th_sensor.Startup();  //starts the sensor-related class
}

void loop() {
  float temp_humid_vals[2] = {0};
  if(th_sensor.get_temp_humid(temp_humid_vals))
  {
    Serial.print("\nCurrent Temperature: ");
    Serial.println(temp_humid_vals[0]);
    Serial.print("Current Humidity: ");
    Serial.println(temp_humid_vals[1]);
  }
  else
  {
    Serial.println("\n\n Couldn't get any data");
  }
}
