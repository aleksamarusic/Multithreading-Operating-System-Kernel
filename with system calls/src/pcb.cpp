#include <dos.h>
#include "pcb.h"
#include "schedule.h"
#include "list.h"
#include "sleepLst.h"
#include <iostream.h>
#include <stdlib.h>



ID PCB::pcbID = 0;


PCB::PCB(StackSize stackSize, Time timeSlice):
    timeSlice(timeSlice), id(++pcbID),
    state(PCB::CREATED), stack(0)
{
	if (stackSize > maxStackSize)
		stackSize = maxStackSize;
	this->stackSize = stackSize;
	blockedOnThread = new List<PCB>();
	//cout<<"Thread with ID "<<id<<" created!"<<endl;
}

PCB::~PCB()
{
	delete blockedOnThread;
	blockedOnThread = 0;
	delete [] stack;
	//cout<<"Thread "<<id<<" done!"<<endl;
}

void PCB::start()
{
	if (getState() == PCB::CREATED)
	{
		this->setState(PCB::READY);
		if (this != Kernel::getIdleThreadPCB() || this != Kernel::getKernelThreadPCB())
			Scheduler::put(this);
	}
}

void PCB::initializeStack(Thread * thread)
{
	static int sizeOfStack;
	sizeOfStack = stackSize / sizeof(unsigned);
	stack = new unsigned[sizeOfStack];
	stack[sizeOfStack-1] = FP_SEG(thread);
	stack[sizeOfStack-2] = FP_OFF(thread);
	stack[sizeOfStack-5] = 0x200;
	stack[sizeOfStack-6] = FP_SEG(Thread::wrapper);
	stack[sizeOfStack-7] = FP_OFF(Thread::wrapper);
	ss = FP_SEG(stack+sizeOfStack-16);
	bp = sp = FP_OFF(stack+sizeOfStack-16);
}
/*
void PCB::wrapper()
{
	PCB::getRunningThread()->getMyThread()->run();
	PCB::getRunningThread()->finishThread();
}*/

void PCB::finishThread()
{
	while(!(blockedOnThread->isEmpty()))
	{
		PCB *waitThread = blockedOnThread->getFirst();
		waitThread->setState(READY);
		Scheduler::put(waitThread);
	}
	delete blockedOnThread;
	blockedOnThread = 0;
	setState(FINISHED);
	dispatch();
}

void PCB::waitToComplete()
{
	if(getState() == PCB::CREATED || getState() == PCB::FINISHED)	return;
	if(this == Kernel::getIdleThreadPCB() || this == Kernel::getRunningThreadPCB() || this == Kernel::getKernelThreadPCB())	return;
	Kernel::getRunningThreadPCB()->setState(BLOCKED);
	blockedOnThread->add(Kernel::getRunningThreadPCB());
	dispatch();
}



