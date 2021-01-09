/*
 Arduino.h - Main include file for the Arduino SDK
 Copyright (c) 2005-2013 Arduino Team.  All right reserved.
 */
#include "Arduino.h"
#include "Sensor.h"

TH_Sensor::TH_Sensor(uint8_t pin) {
    DATA_PIN = pin;
    _count = CPU_SPEED();
}

void TH_Sensor::Startup(void) {
    // recommended: set PIN 16; 17 to LOW when using I/O on the X-Bee-Carrier
    pinMode(16, INPUT);
    pinMode(17, INPUT);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    // setup our pin
    /*TODO(Nils): We shouldnt have to write that high all the time.
      not doing so will save power and we can probably just wait
      some time and treat the sensor like it was always on.

      I think can replace this with
      pinMode(DATA_PIN, OUTPUT);
      digitalWrite(DATA_PIN, LOW);
    */
    pinMode(DATA_PIN, INPUT);
    digitalWrite(DATA_PIN, HIGH);
}

/*
 * TODO(Nils): Go recheck if that makes any sense, since we sometimes
 * get false values. Then we should maybe just try again and select the answer
 * we get most out of the once we have recieved.
 */
bool TH_Sensor::get_temp_humid(float* temp_humdity_vals) {
    // try to get the data from the sensor
    // TODO(Nils) implement a sensor startup here to save power (see above)
    for (uint8_t i=0; !sensorReadout(); i++) {
        if (i > 2) {
            return false;
        }
        delay(2000);
    }
    // temperature is in data[2] and humidity in data[0]
    // copy temperature and humidity values
    temp_humdity_vals[0] = _data[2];   // stores temperature in data[0]
    temp_humdity_vals[1] = _data[0];   // stores humidty in data[1]
    return true;
}

/*TODO(Nils):
 * We might want to set counter to a higher number and then change its type to INT, since this
 * could potenially improve the success-rate of our reading process (should kind of depent on the CPU).
 */
boolean TH_Sensor::sensorReadout(void) {
    boolean success = false;       // Flag for validity of the data
    boolean laststate = HIGH;  // We use this to detect the bitstates
    uint8_t counter = 0;       // counts time while reading the bits
    uint8_t i = 0;             // index
    uint8_t j = 0;             // index

    // TODO(Nils): I assume this is enough for the sensor to startup
    // so we dont need a the data_pin high before
    digitalWrite(DATA_PIN, HIGH);
    delay(250);

    // get the current time to check wether we can read the sensor
    int32_t current_reading = millis();
    // System-Time will wrap every ~50 days
    if (current_reading < _last_reading) {
        _last_reading = 0;
    }
    // test for delay inbetween measurements
    if ((current_reading - _last_reading + 250) < MIN_DELAY) {
        return false;
    }
    // new last reading_time
    _last_reading = millis();

    // start reading process
    // delete data
    _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;

    // blocks all system interrupts during sensor reading
    cli();

    // startup sequence:
    // http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/ (IMG)
    pinMode(DATA_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
    delay(20);
    digitalWrite(DATA_PIN, HIGH);
    delayMicroseconds(40);

    // start reading
    pinMode(DATA_PIN, INPUT);
    // loop for as many bits as we get from the Sensor
    for (i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        // wait for a state change from the input
        while (digitalRead(DATA_PIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(DATA_PIN);
        if (counter == 255) {
            break;
        }
        // ignore first 3 transitions
        if ((i >= 4) && (i % 2 == 0)) {
            // shove each bit into the storage bytes
            _data[j / 8] <<= 1;
            if (counter > _count) {
                _data[j / 8] |= 1;
            }
            j++;
        }
    }
    // allow system interrupts from here on out
    sei();
    // check that we read all 40 bits and that the checksum is correct
    check_sum = (_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF)
    if ((j >= 40) && (_data[4] == check_sum) {
        success = true;
    }
    return success;
}

uint8_t TH_Sensor::CPU_SPEED(void) {
  if ((F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)) {
  // 8 MHz(ish) AVR ---------------------------------------------------------
     return 3;
  } else if ((F_CPU >= 15400000UL) && (F_CPU <= 19000000L)) {
  // 16 MHz(ish) AVR --------------------------------------------------------
      return 6;
  } else if (F_CPU == 48000000UL) {
  // 48MHz SAMD21J18A (Sodaq Explorer)
      return 18;
  } else if (F_CPU == 168000000L) {
  // 168MHz STM32F405 STM32F407
    return 40;
  } else if (F_CPU == 80000000L) {
    return 22;
  } else if (F_CPU == 160000000L) {
    return 36;
  } else {
    // ERROR "CPU SPEED NOT SUPPORTED"
    return 25;
  }
}
