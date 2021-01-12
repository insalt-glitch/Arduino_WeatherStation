
struct TH_Sensor {
  // constructor, pass the DATA-PIN
  TH_Sensor(uint8_t pin);
  // call on SETUP-FUNCTION
  void Startup(void);
  // get the current sensor data: data[0]=TEMPERATURE data[1]=HUMIDITY
  boolean get_temp_humid(float* data);

 private:
  // data-members
  // timing transitions we need to keep track of. 2 * number bits + extra
  const uint8_t MAXTIMINGS = 85;
  // http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info
  uint8_t DATA_PIN;
  // Processor dependent
  uint8_t _count;
  // Stores data we get from our sensor
  uint8_t _data[6];

  // minimum delay between reading-cycles in milliseconds
  const uint16_t MIN_DELAY = 2000;
  // last time a value has been read from the sensor
  int32_t _last_reading = 0;

  // functions
  // reading out data from the sensor
  boolean sensorReadout(void);
  // determines the counter based on the used CPU
  uint8_t CPU_SPEED(void);
};
