#include "serial.h"

byte serialByte;


extern void readSerialBus() {
	if(Serial.available()) {
		serialByte = Serial.read();
	}
	else {
		serialByte = 0;
	}

	// if(serialByte == ) { // eigen protocol afhandeling kan hier
}
