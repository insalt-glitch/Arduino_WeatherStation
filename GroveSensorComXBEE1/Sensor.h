
class TH_Sensor {
  //data-members
  const uint8_t MAXTIMINGS = 85;       // how many timing transitions we need to keep track of. 2 * number bits + extra
	uint8_t DATA_PIN;					           // see http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ for info
  uint8_t _count;                      // Processor dependent  
  uint8_t _data[6];                    // Stores data we get from our sensor
  
	const uint16_t MIN_DELAY = 2000;		 // minimum delay between reading-cycles in milliseconds
	unsigned long int _last_reading = 0; // last time a value has been read from the sensor


  //functions
	boolean readingProcess(void);		     // reading out data from the sensor
	uint8_t CPU_SPEED(void);			       // determines the counter based on the used CPU
	
public:
	TH_Sensor(uint8_t pin);              // cunstructor, pass the DATA-PIN
  void start(void);                    // call on SETUP-FUNCTION
	boolean get_temp_humid(float* data); // get the current temperature and humidity data[0]=TEMPERATURE data[1]=HUMIDITY
};
