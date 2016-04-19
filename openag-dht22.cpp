/** 
 *  \file openag-dht22.cpp
 *  \brief Sensor module for air temperature and humidity.
 *  \details See openag-dht22.h for details.
 */
#include "openag-dht22.h"

Dht22::Dht22(String temperature_id, String humidity_id, int analog_pin) {
  _temperature_id = temperature_id;
  _humidity_id = humidity_id;
  _analog_pin = analog_pin;
}

void Dht22::begin(void) {
  pinMode(_analog_pin, INPUT);
  digitalWrite(_analog_pin, HIGH);
  _count = COUNT;
  _first_reading = true;
  _last_read_time = 0;
}

String Dht22::get(String id) {
  String msg = "";
  if (id == _temperature_id) {
    getSensorData();
    msg = id + ":" + String(temperature, 1);
  }
  else if (id == _humidity_id) {
    getSensorData();
    msg = id + ":" + String(humidity, 1);
  }
  return msg;
}
  
String Dht22::set(String id, String val) {
  return "";
}

void Dht22::getSensorData(void) {
  humidity = 0;
  temperature = 0;
  if (read()) {
    humidity = _data[0];
    humidity *= 256;
    humidity += _data[1];
    humidity /= 10;
    
    temperature = _data[2] & 0x7F;
    temperature *= 256;
    temperature += _data[3];
    temperature /= 10;
    if (_data[2] & 0x80) {
      temperature *= -1;
    }    
  }
}

boolean Dht22::read(void) {
  uint8_t last_state = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long current_time;

  digitalWrite(_analog_pin, HIGH);
  delay(2); // old delay time was 250

  current_time = millis();
  if (current_time < _last_read_time) {
    // ie there was a rollover
    _last_read_time = 0;
  }
  if (!_first_reading && ((current_time - _last_read_time) < 2000)) {
    return true; // return last correct measurement
  }
  _first_reading = false;
  _last_read_time = millis();

  _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;
  
  // now pull it low for ~20 milliseconds
  pinMode(_analog_pin, OUTPUT);
  digitalWrite(_analog_pin, LOW);
  delay(20);
  digitalWrite(_analog_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_analog_pin, INPUT);

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (digitalRead(_analog_pin) == last_state) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    last_state = digitalRead(_analog_pin);

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      _data[j/8] <<= 1;
      if (counter > _count)
        _data[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits and that the checksum matches
  if ((j >= 40) && 
      (_data[4] == ((_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF)) ) {
    return true;
  }
  return false;
}
