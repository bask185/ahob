#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "roundRobinTasks.h"
#include "railCrossing.h"
#include "readTrack1.h"
#include "readTrack2.h"

void setup() {
	initTimers();
	initIO();
	Serial.begin(115200);
	railCrossingInit();
	readTrack1Init();
	readTrack2Init();
}

void loop() {
	processRoundRobinTasks();
	
	railCrossing();
	readTrack1();
	readTrack2();
}