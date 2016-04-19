/** 
 *  \file dht22.h
 *  \brief Sensor module for air temperature and humidity.
 */

#ifndef OPENAG_DHT22_H
#define OPENAG_DHT22_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// 8 MHz(ish) AVR ---------------------------------------------------------
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
#define COUNT 3
// 16 MHz(ish) AVR --------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
#define COUNT 6
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

/** 
 *  \brief Sensor module for air temperature and humidity.
 */
class Dht22 {
  public:
    // Public Functions
    Dht22(String temperature_id, String humidity_id, int analog_pin);
    void begin(void);
    String get(String id);
    String set(String id, String val);

    // Public Variables
    float humidity;
    float temperature;
    
  private:
    // Private Functions
    void getSensorData(void);
    boolean read(void);
    
    // Private Variables
    String _temperature_id;
    String _humidity_id;
    int _analog_pin;
    uint8_t _data[6];
    uint8_t _count;
    uint32_t _last_read_time;
    boolean _first_reading;
};

#endif

