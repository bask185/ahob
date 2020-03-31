
#include "roundRobinTasks.h"
#include "src/basics/io.h"

extern void processRoundRobinTasks(void) {
	static unsigned char taskCounter = 0;

// HIGH PRIORITY ROUND ROBIN TASKS
	//readSerialBus();
	//updateIO();

// LOW PRIORITY ROUND ROBIN TASKS
	taskCounter ++;
	switch(taskCounter) {
		default: taskCounter = 0;

		case 0:
		/* fill in a task */
		break;

		case 1:
		/* fill in a task */
		break; } }