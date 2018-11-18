#include "schedule.h"
#include "thread.h"
#include <iostream.h>

Thread::Thread (StackSize stackSize, Time timeSlice)
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_CONSTRUCTOR;
	systemCallParams.params[0] = stackSize;
	systemCallParams.params[1] = timeSlice;
	systemCallParams.params[2] = (unsigned long)(this);

	Kernel::systemCallPrepare(&systemCallParams);

	myID = systemCallParams.returnValue;
	 	
	unlock
}

Thread::~Thread ()
{
	lock
	waitToComplete();
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_DESTRUCTOR;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

void Thread::start()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_START;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

/*
Thread* Thread::getThreadById(ID id)
{
	lock
	Thread* ret = 0;
	PCB* pcb = PCB::getPcbById(id);
	if (pcb) ret = pcb->getMyThread();
	 	
	Sunlock(1)
	return ret;
}*/

void Thread::waitToComplete()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_WAIT_TO_COMPLETE;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

void Thread::sleep(Time timeToSleep)
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_SLEEP;
	systemCallParams.params[0] = timeToSleep;
	
	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

void dispatch()
{
	lock
	Kernel::explicitDispatchRequest = 1;
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SYSTEM_DISPATCH;
	Kernel::systemCallPrepare(&systemCallParams);
	unlock
}

void Thread::wrapper(Thread* runningThread)
{
	runningThread->run();
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = THREAD_FINISH_THREAD;
	systemCallParams.params[0] = runningThread->getID();

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}