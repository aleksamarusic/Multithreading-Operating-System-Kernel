#include <iostream.h>
#include <dos.h>
#include "timer.h"
#include "pcb.h"
#include "schedule.h"



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
	static volatile unsigned tsp, tss, tbp;
	if(!PCB::explicitDispatchRequest) 
	{
		oldRoutine();
		tick();
		PCB::updateSleepWaitList();
		if(PCB::getRunningThread()->timeSlice != 0) 
			timeCounter--;
		if(timeCounter > 0 || PCB::getRunningThread()->timeSlice == 0)
			return;
	}
	PCB::explicitDispatchRequest = 0;
	
	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}
	PCB::getRunningThread()->sp = tsp;
	PCB::getRunningThread()->ss = tss;
	PCB::getRunningThread()->bp = tbp;

	if(PCB::getRunningThread()->getState() == PCB::READY && PCB::getRunningThread() != PCB::getIdleThread())
		Scheduler::put(PCB::getRunningThread());
	PCB::setRunningThread(Scheduler::get());
	if(!PCB::getRunningThread())
		PCB::setRunningThread(PCB::getIdleThread());

	tsp = PCB::getRunningThread()->sp;
	tss = PCB::getRunningThread()->ss; 
	tbp = PCB::getRunningThread()->bp;
	
	timeCounter = PCB::getRunningThread()->timeSlice;

	asm {
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}
}






