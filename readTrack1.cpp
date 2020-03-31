// HEADER FILES
#include <Arduino.h>
#include "readTrack1.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"

// MACROS
#define stateFunction(x) static bool x##F(void)
#define entryState if(runOnce) 
#define onState runOnce = false; if(!runOnce)
#define exitState if(!exitFlag) return false; else
#define State(x) break; case x: if(runOnce) Serial.println("    "#x); if(x##F())
#define STATE_MACHINE_BEGIN if(!enabled) { \
	if(!readTrack1T) enabled = true; } \
else switch(state){\
	default: Serial.println("unknown state executed, state is idle now"); state = readTrack1IDLE; case readTrack1IDLE: return true;
#define STATE_MACHINE_END break;}return false;


#define beginState monitorSw1Sw2
#ifndef beginState
#error beginState not yet defined
#endif

// VARIABLES
#define DET1 1
#define DET2 2

static unsigned char state = beginState, priority;
static bool enabled = true, runOnce = true, exitFlag = false;

unsigned char track1occupied;

// FUNCTIONS
extern void readTrack1Init(void) { state = beginState; }
extern byte readTrack1GetState(void) { return state;}
extern void readTrack1SetState(unsigned char _state) { state = _state; runOnce = true; }
static void nextState(unsigned char _state, unsigned char _interval) {
	runOnce = true;
	exitFlag = false;
	if(_interval) {
		enabled = false;
		readTrack1T = _interval; } 
	state = _state; }

// STATE FUNCTIONS
stateFunction(monitorSw1Sw2) {
	entryState {
		track1occupied = 0;
	}
	onState {
		if(digitalRead(detector1) == false) {
			priority = DET1;
			exitFlag = true;
		}
		if(digitalRead(detector2) == false) {
			priority = DET2;
			exitFlag = true;
		}
	}
	exitState {
		track1occupied = 1;
		if(priority == DET1) Serial.println("    detector 1 made first");
		else				 Serial.println("    detector 2 made first");
		return true;
	}
}

stateFunction(waitSw1) {
	entryState {
		
	}
	onState {
		if(digitalRead(detector1) == false)  {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("    train reached detector 1");
		return true;
	}
}

stateFunction(waitSw2) {
	entryState {
		
	}
	onState {
		if(digitalRead(detector2) == false) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("    train reached detector 2");
		return true;
	}
}

stateFunction(waitTrain2passSw1) {
	entryState {
		readTrack1T = 10;
	}
	onState {
		if(digitalRead(detector1) == false) {
			readTrack1T = 10; // train must be free for atleast 5 seconds
		}

		if(!readTrack1T) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("    train has passed detector 1");
		return true;
	}
}

stateFunction(waitTrain2passSw2) {
	entryState {
		readTrack1T = 10;
	}
	onState {
		if(digitalRead(detector2) == false) {
			readTrack1T = 10; // train must be free for atleast 5 seconds
		}

		if(!readTrack1T) {
			exitFlag = true;
		}
	}
	exitState {
		Serial.println("    train has passed detector 2");
		return true;
	}
}

// STATE MACHINE
extern bool readTrack1(void) {
	STATE_MACHINE_BEGIN

	State(monitorSw1Sw2) {
		if(priority == DET2) nextState(waitSw1, 2);
		else				 nextState(waitSw2, 2); }

	State(waitSw1) {
		nextState(waitTrain2passSw1, 0); }

	State(waitSw2) {
		nextState(waitTrain2passSw2, 0); }

	State(waitTrain2passSw1) {
		nextState(monitorSw1Sw2, 2); }

	State(waitTrain2passSw2) {
		nextState(monitorSw1Sw2, 2); }

	STATE_MACHINE_END
}
