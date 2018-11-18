#include <iostream.h>
#include <dos.h>
#include "timer.h"
#include "schedule.h"
#include "kernel.h"
#include "thread.h"

/*
Time Timer::timeCounter = 0;
InterruptRoutinePointer Timer::oldRoutine = 0;


void Timer::timerInit()
{
	oldRoutine = getvect(8);
	setvect(8, &timer);
	setvect(0x60, oldRoutine);
	resetCounter();
}

void Timer::timerRestore()
{
	setvect(8, oldRoutine);
}

void Timer::resetCounter()
{
	timeCounter = 0;
}

void interrupt Timer::timer(...)
{
	if (Kernel::inSystemCall == 0)
	{
		oldRoutine();
		tick();
		Kernel::updateSleepWaitList();
		if(Kernel::getRunningThreadPCB()->timeSlice != 0) 
			timeCounter--;
		if(timeCounter > 0 || Kernel::getRunningThreadPCB()->timeSlice == 0)
			return;
		dispatch();
	}
	else
	{
		Kernel::callTimer = 1;
	}
}

*/




