#include "pcb.h"
#include "schedule.h"
#include "timer.h"
#include <iostream.h>

Thread::Thread (StackSize stackSize, Time timeSlice)
{
	lock
	myPCB=new PCB(stackSize, timeSlice, this);
	unlock
}

Thread::~Thread ()
{
	lock
	waitToComplete();
	delete myPCB;
	unlock
}

void Thread::start()
{
	lock
	myPCB->start();
	unlock
}

Thread* Thread::getThreadById(ID id)
{
	lock
	Thread* ret = 0;
	PCB* pcb = PCB::getPcbById(id);
	if (pcb) ret = pcb->getMyThread();
	unlock
	return ret;
}

ID Thread::getRunningId()
{
	lock
	ID idRunning = PCB::getRunningThread() -> getID();
	unlock
	return idRunning;
}

ID Thread::getId()
{
	lock
	ID id = myPCB->getID();
	unlock
	return id;
}

void Thread::waitToComplete()
{
	myPCB->waitToComplete();
}

void Thread::sleep(Time timeToSleep)
{
	lock
	PCB::sleep(timeToSleep);
	unlock
}

void dispatch()
{
	lock
	PCB::explicitDispatchRequest = 1;
	Timer::timer();
	//PCB::explicitDispatchRequest = 0;
	unlock
}