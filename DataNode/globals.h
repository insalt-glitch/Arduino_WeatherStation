

//return values 0: ok, -1: error, 1: nothing , 2: modified
#define OK 0
#define ERR -1 
#define NOTHING 1
#define MODIFIED 2
#define GDO0 2 // used for polling the RF received data
#define LEDon digitalWrite(9,HIGH);
#define LEDoff digitalWrite(9,LOW);

#define logAddress 1  // dirección de Arduino Maestro Logger
#define mesa00Addr 2  // dirección de RFBee mesa 0°
#define mesa15Addr 3  // dirección de RFBee mesa 15°
#define mesa23Addr 4  // dirección de RFBee mesa 23°
#define mesa45Addr 5  // dirección de RFBee mesa 45°
