// HEADER FILES
#include <Arduino.h>
#include "readTrack2.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"

// MACROS
#define stateFunction(x) static bool x##F(void)
#define entryState if(runOnce) 
#define onState runOnce = false; if(!runOnce)
#define exitState if(!exitFlag) return false; else
#define State(x) break; case x: if(runOnce) Serial.println("        "#x); if(x##F())
#define STATE_MACHINE_BEGIN if(!enabled) { \
	if(!readTrack2T) enabled = true; } \
else switch(state){\
	default: Serial.println("unknown state executed, state is idle now"); state = readTrack2IDLE; case readTrack2IDLE: return true;
#define STATE_MACHINE_END break;}return false;


#define beginState monitorSw3Sw4
#ifndef beginState
#error beginState not yet defined
#endif

// VARIABLES
#define DET3 1
#define DET4 2

static unsigned char state = beginState, priority;
static bool enabled = true, runOnce = true, exitFlag = false;

unsigned char track2occupied;

// FUNCTIONS
extern void readTrack2Init(void) { state = beginState; }
extern byte readTrack2GetState(void) { return state;}
extern void readTrack2SetState(unsigned char _state) { state = _state; runOnce = true; }
static void nextState(unsigned char _state, unsigned char _interval) {
	runOnce = true;
	exitFlag = false;
	if(_interval) {
		enabled = false;
		readTrack2T = _interval; } 
	state = _state; }

// STATE FUNCTIONS
stateFunction(monitorSw3Sw4) {
	entryState {
		track2occupied = 0;
	}
	onState {
		if(digitalRead(detector3) == false) {
			priority = DET3;
			exitFlag = true;
		}
		if(digitalRead(detector4) == false ) {
			priority = DET4;
			exitFlag = true;
		}
	}
	exitState {
		track2occupied = 1;
		if(priority == DET3) Serial.println("        detector 3 made first");
		else				 Serial.println("        detector 4 made first");
		return true;
	}
}


stateFunction(waitSw3) {
	entryState {
		
	}
	onState {
		if(digitalRead(detector3) == false)  {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("        train reached detector 3");
		return true;
	}
}

stateFunction(waitSw4) {
	entryState {
		
	}
	onState {
		if(digitalRead(detector4) == false) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("        train reached detector 4");
		return true;
	}
}

stateFunction(waitTrain2passSw3) {
	entryState {
		readTrack2T = 10;
	}
	onState {
		if(digitalRead(detector3) == false) {
			readTrack2T = 10; // train must be free for atleast 5 seconds
		}

		if(!readTrack2T) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("        train has passed detector 3");
		return true;
	}
}

stateFunction(waitTrain2passSw4) {
	entryState {
		readTrack2T = 10;
	}
	onState {
		if(digitalRead(detector4) == false) {
			readTrack2T = 10; // train must be free for atleast 5 seconds
		}

		if(!readTrack2T) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("        train has passed detector 4");
		return true;
	}
}

// STATE MACHINE
extern bool readTrack2(void) {
	STATE_MACHINE_BEGIN

	State(monitorSw3Sw4) {
		if(priority == DET4) nextState(waitSw3, 2);
		else				 nextState(waitSw4, 2); }

	State(waitSw3) {
		nextState(waitTrain2passSw3, 0); }

	State(waitSw4) {
		nextState(waitTrain2passSw4, 0); }

	State(waitTrain2passSw3) {
		nextState(monitorSw3Sw4, 2); }

	State(waitTrain2passSw4) {
		nextState(monitorSw3Sw4, 2); }

	STATE_MACHINE_END
}
