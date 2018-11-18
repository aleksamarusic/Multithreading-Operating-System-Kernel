#include "schedule.h"
#include "kernsem.h"
#include "pcb.h"
#include "list.h"

KernelSem::KernelSem(int init) : value(init)
{
	list = new List();
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
		PCB::getRunningThread()->setState(PCB::BLOCKED);
		list->add(PCB::getRunningThread());
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


