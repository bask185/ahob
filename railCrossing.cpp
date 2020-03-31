// HEADER FILES
#include <Arduino.h>
#include "railCrossing.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "config.h"
#include "readTrack1.h"
#include "readTrack2.h"
#include <Servo.h>

// MACROS
#define stateFunction(x) static bool x##F(void)
#define entryState if(runOnce) 
#define onState runOnce = false; if(!runOnce)
#define exitState if(!exitFlag) return false; else
#define State(x) break; case x: if(runOnce) Serial.println(#x); if(x##F())
#define STATE_MACHINE_BEGIN if(!enabled) { \
	if(!railCrossingT) enabled = true; } \
else switch(state){\
	default: Serial.println("unknown state executed, state is idle now"); state = railCrossingIDLE; case railCrossingIDLE: return true;
#define STATE_MACHINE_END break;}return false;


#define beginState waitCrossing
#ifndef beginState
#error beginState not yet defined
#endif

// VARIABLES
static unsigned char state = beginState;
static bool enabled = true, runOnce = true, exitFlag = false;

uint8_t angle;
Servo arm1;
Servo arm2;

// FUNCTIONS
extern void railCrossingInit(void) { 
	state = beginState; 
	arm1.write(openPos);
	arm2.write(openPos);
}
extern byte railCrossingGetState(void) { return state;}
extern void railCrossingSetState(unsigned char _state) { state = _state; runOnce = true; }
static void nextState(unsigned char _state, unsigned char _interval) {
	runOnce = true;
	exitFlag = false;
	if(_interval) {
		enabled = false;
		railCrossingT = _interval; } 
	state = _state; }


void blinkLeds(uint8_t blinkTime) {
	if(!blinkT) {
		blinkT = blinkTime; // 0.5s

		if(digitalRead(blinkLed1)) {
			digitalWrite(blinkLed1, LOW);
			digitalWrite(blinkLed2, HIGH);
		}
		else {
			digitalWrite(blinkLed1, HIGH);
			digitalWrite(blinkLed2, LOW);
		}
	}
}

void controlArms(int8_t direction) {
	if(!servoT) {
		static byte prevAngle;
		servoT = (armSpeed / 90); // armspeed in ms
		
		if(direction == 1) { // close
			if(angle < closePos) angle++;
			arm1.write(angle);
			arm2.write(angle);
		}
		else {
			if(angle > openPos) angle --;
			arm1.write(angle);
			arm2.write(angle);
		}
		if(prevAngle != angle) {
			Serial.print("angle = ");Serial.println(angle);
			prevAngle = angle;
		}
	}
}



// STATE FUNCTIONS
stateFunction(closeCrossing) {
	entryState {
		digitalWrite(led1, HIGH);
	}
	onState {
		blinkLeds(BLINK_TIME);
		controlArms(1);

		if(track1occupied == false && track2occupied == false) {
			exitFlag = true;
		}
	}
	exitState {

		return true;
	}
}

stateFunction(openCrossing) {
	entryState {
	}
	onState {
		blinkLeds(BLINK_TIME);
		controlArms(2);

		if(angle == openPos || track1occupied || track2occupied) { 
			exitFlag = true;
		}
	}
	exitState {
		digitalWrite(led1, LOW);
		digitalWrite(blinkLed1, LOW);
		digitalWrite(blinkLed2, LOW);
		return true;
	}
}

stateFunction(waitCrossing) {
	entryState {
	}
	onState {
		if(track1occupied || track2occupied) {
			exitFlag = true;
		}
	}
	exitState {

		return true;
	}
}

// STATE MACHINE
extern bool railCrossing(void) {
	STATE_MACHINE_BEGIN

	State(closeCrossing) {
		nextState(openCrossing, 0); }

	State(openCrossing) {
		if(angle == 0)	nextState(waitCrossing, 0);
		else			nextState(closeCrossing, 0); }

	State(waitCrossing) {
		nextState(closeCrossing, 0); }

	STATE_MACHINE_END
}



/*
// stateMachines.cpp
#include "stateMachines.h"
// generic macros can be used for all state machine
#define State(x) break; case x: if(x##F() 
#define stateFunction(x) uint8_t x##F()
#define STATE_MACHINE_BEGIN switch(state) { default:
#define STATE_MACHINE_END break; }


// dublicate stuff
enum states {
	x,
	y,
	z,
};

uint8_t state, runOnce;

void nextState(uint8_t _state) { 
	state = _state;
	runOnce = 1;
}

// UNIQUE STUFF FOR STATE MACHINE 1:
// state functions
stateFunction(x) { // expands to uint8_t xF()
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

stateFunction(y) { // expands to uint8_t yF()
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

stateFunction(z) {
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

// state machine 1
void stateMachine1() {
	switch(state) {
		STATE_MACHINE_BEGIN // expands to switch(state) { default:

		State(x) { // expands to break; case x: if(x##F()) 
			nextState(y); }

		State(y) {
			nextState(z); }

		State(z) {
			nextState(x); }

		STATE_MACHINE_END
	}
}

// dublicate stuff
enum states {
	x,
	y,
	z,
};

uint8_t state, runOnce;

void nextState(uint8_t _state) { 
	state = _state;
	runOnce = 1;
}

// UNIQUE STUFF FOR STATE MACHINE 2:
// state functions
stateFunction(a) { // expands to uint8_t aF()
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

stateFunction(b) {
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

stateFunction(c) {
	if(runOnce) {
		runOnce = 0;
		// run once code 
	}
	// code
	if(/* condition * / ) return 1;
	else return 0;
}

// state machine 2
void stateMachine2() {
	STATE_MACHINE_BEGIN

	State(a) {
		nextState(b); }

	State(b) {
		nextState(c); }

	State(c) {
		nextState(a); }

	STATE_MACHINE_END
}

inline void callStateMachines() { // called from main()
	stateMachine1();
	stateMachine2();
}

// main.cpp
int main() {
	bool PIGS_CAN_FLY = true;

	while(PIGS_CAN_FLY == true) {
		callStateMachines();
	}
	return 0;
}

*/