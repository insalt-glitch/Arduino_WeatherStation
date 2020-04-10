#include "Arduino.h"
#include "Sensor.h"

  //global variables
  const int DATA_PIN = 5; //see http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info ( maybe it's 6)

//class creation
TH_Sensor th_sensor(DATA_PIN); //creates the object to talk to the sensor

void setup() {
    //recommended: set PIN 16; 17 to LOW when using I/O on the X-Bee-Carrier
    pinMode(16, INPUT);
    pinMode(17, INPUT);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    Serial.begin(115200); // sets communication speed
    
    th_sensor.start();  //starts the sensor-related class   
    Serial.println("Startup!");//print success
    delay(1000); //temporary delay
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
