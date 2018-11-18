#include "kernev.h"
#include "pcb.h"
#include "schedule.h"
#include "IVTEntry.h"

ID KernelEv::kernelEvID = 0;

KernelEv::KernelEv(IVTNo ivtNo)
{
	id = kernelEvID++;
	value = 0;
	IVTNumber = ivtNo;
	eventOwner = Kernel::getRunningThreadPCB();
	blocked = 0;
	Kernel::IVTable[ivtNo]->setKernelEvent(this);
}

KernelEv::~KernelEv()
{
	signal();
	Kernel::IVTable[IVTNumber]->removeKernelEvent();
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
	if(Kernel::getRunningThreadPCB() != eventOwner)
		return;
	if(value == 1)
		value = 0;
	else
	{
		Kernel::getRunningThreadPCB()->setState(PCB::BLOCKED);
		blocked = 1;
		dispatch();
	}
}



