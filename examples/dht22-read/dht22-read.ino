#include <openag-dht22.h>

Dht22 dht22("air_temperature", "humidity", A0);


void setup() {
	dht22.begin();
	Serial.begin(9600);
}

void loop() {
	Serial.println(dht22.get("air_temperature"));
	Serial.println(dht22.get("humidity"));
	delay(2000);
}