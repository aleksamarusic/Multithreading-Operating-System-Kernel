#include "kernev.h"
#include "pcb.h"
#include "schedule.h"
#include "IVTEntry.h"


KernelEv::KernelEv(IVTNo ivtNo)
{
	value = 0;
	this->ivtNo = ivtNo;
	eventOwner = PCB::getRunningThread();
	blocked = 0;
	IVTEntry::IVTable[ivtNo]->setKernelEvent(this);
}

KernelEv::~KernelEv()
{
	signal();
	IVTEntry::IVTable[ivtNo]->removeKernelEvent();
}

void KernelEv::signal()
{
	if(blocked == 0)
		value = 1;	
	else
	{
		eventOwner->setState(PCB::READY);
		Scheduler::put(eventOwner);
		blocked = 0;
		dispatch();
	}
}

void KernelEv::wait()
{
	if(PCB::getRunningThread() != eventOwner)
		return;
	if(value == 1)
		value = 0;
	else
	{
		PCB::getRunningThread()->setState(PCB::BLOCKED);
		blocked = 1;
		dispatch();
	}
}



