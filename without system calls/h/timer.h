#ifndef _TIMER_H_
#define _TIMER_H_

#include "global.h"

class Timer
{
public:
	static void timerInit();
	static void timerRestore();
	static void resetCounter();

	static Time timeCounter;
	static InterruptRoutinePointer oldRoutine;	

	/*
	interrupt routine that is placed in 8th entry of Interrupt Vector Table
	at initialisation. Enables context switch every 55ms, and is responsible for
	system's time-sharing concept.

	interrupt modifier saves all the registers (necessery for safe context switch)
	, and disables interrupts when routine is called implicitly.
	*/
	static void interrupt timer(...);
};


#endif
