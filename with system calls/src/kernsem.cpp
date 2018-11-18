#include "schedule.h"
#include "kernsem.h"
#include "pcb.h"

ID KernelSem::kernelSemID = 0;

KernelSem::KernelSem(int init) : value(init), id(kernelSemID++)
{
	list = new List<PCB>();
}

KernelSem::~KernelSem()
{
	while (value<0)
		signal();
	delete list;
}

int KernelSem::wait(int toBlock)
{

	if (value <= 0 && toBlock != 0)
	{
		value--;
		Kernel::getRunningThreadPCB()->setState(PCB::BLOCKED);
		list->add(Kernel::getRunningThreadPCB());
		dispatch();
		return 1;
	}
	else if(value > 0)
	{
		value--;
		return 0;
	}
	else
		return -1;
}

void KernelSem::signal()
{
	if (value++<0)
	{
		PCB* pointerPCB=list->getFirst();
		if (pointerPCB)
		{
			pointerPCB->setState(PCB::READY);
			Scheduler::put(pointerPCB);
		}
	}
}


