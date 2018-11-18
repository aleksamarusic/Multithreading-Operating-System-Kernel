#include <dos.h>
#include "pcb.h"
#include "schedule.h"
#include "list.h"
#include "sleepLst.h"
#include <iostream.h>
#include <stdlib.h>



ID PCB::pcbID = 0;
PCB *PCB::mainThreadPCB = 0;
PCB *PCB::running = 0;
PCB* PCB::idlePCB = 0;
List *PCB::allPCBs = new List();
SleepList *PCB::blockedOnSleep = new SleepList();
volatile unsigned PCB::explicitDispatchRequest = 0;


PCB::PCB(StackSize stackSize, Time timeSlice,Thread* thread):
    myThread(thread), timeSlice(timeSlice), id(++pcbID),
    state(PCB::CREATED), stack(0)
{
	if (stackSize > maxStackSize)
		stackSize = maxStackSize;
	this->stackSize = stackSize;
	blockedOnThread = new List();
	initializeStack();
	//cout<<"Thread with ID "<<id<<" created!"<<endl;
}

PCB::~PCB()
{
	allPCBs->removeFromList(this);
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
		if (this != PCB::getIdleThread())
			Scheduler::put(this);
	}
}

void PCB::initializeStack()
{
	static int sizeOfStack;
	sizeOfStack = stackSize / sizeof(unsigned);
	stack = new unsigned[sizeOfStack];
	stack[sizeOfStack-1] = 0x200;
	stack[sizeOfStack-2] = FP_SEG(&wrapper);
	stack[sizeOfStack-3] = FP_OFF(&wrapper);
	ss = FP_SEG(stack+sizeOfStack-12);
	sp = bp = FP_OFF(stack+sizeOfStack-12);
}

void PCB::wrapper()
{
	PCB::getRunningThread()->getMyThread()->run();
	PCB::getRunningThread()->finishThread();
}

void PCB::sleep(Time timeToSleep)
{
	PCB::getRunningThread()->setState(PCB::BLOCKED);
	blockedOnSleep->insert(getRunningThread(), timeToSleep);
	dispatch();
}

void PCB::finishThread()
{
	lock
	while(!(blockedOnThread->isEmpty()))
	{
		PCB *waitThread = blockedOnThread->getFirst();
		waitThread->setState(READY);
		Scheduler::put(waitThread);
	}
	delete blockedOnThread;
	blockedOnThread = 0;
	setState(FINISHED);
	unlock
	dispatch();
}

void PCB::waitToComplete()
{
	if(getState() == PCB::CREATED || getState() == PCB::FINISHED)	return;
	if(this == PCB::getIdleThread() || this == PCB::getRunningThread())	return;
	getRunningThread()->setState(BLOCKED);
	blockedOnThread->add(getRunningThread());
	dispatch();
}

PCB* PCB::getPcbById(ID pcbID)
{
	return allPCBs->getThreadById(pcbID);
}



void PCB::updateSleepWaitList()
{
	lock
	blockedOnSleep->decrease();
	unlock
}
