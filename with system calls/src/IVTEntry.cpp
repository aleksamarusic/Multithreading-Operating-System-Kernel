#include "IVTEntry.h"
#include <dos.h>
#include "KernEv.h"
#include "kernel.h"



IVTEntry::IVTEntry(IVTNo entry, InterruptRoutinePointer newInterruptRoutine)
{
	lock
	IVTNumber = entry;
	oldRoutine = getvect(entry);
	setvect(entry, newInterruptRoutine);
	Kernel::eventsIVT[entry] = 0;
	Kernel::IVTable[entry] = this;
	unlock
}

IVTEntry::~IVTEntry()
{
	signal();
	Kernel::IVTable[IVTNumber] = 0;
	Kernel::eventsIVT[IVTNumber] = 0;
	setvect(IVTNumber, oldRoutine);
	//callOldRoutine();
}

void IVTEntry::setKernelEvent(KernelEv* event)
{
	Kernel::eventsIVT[IVTNumber] = event;
}

void IVTEntry::removeKernelEvent()
{
	Kernel::eventsIVT[IVTNumber] = 0;
}

void IVTEntry::callOldRoutine()
{
	lock
	if(oldRoutine)
		oldRoutine();
	unlock
}

void IVTEntry::signal()
{
	if (Kernel::inSystemCall)
		Kernel::callIVT[IVTNumber] = 1;
	else
	{
		Kernel::callIVT[IVTNumber] = 0;
		if(Kernel::eventsIVT[IVTNumber] != 0) 
			Kernel::eventsIVT[IVTNumber]->signal();
	}
}
/*
IVTEntry * IVTEntry::getIVTEntry(IVTNo ivtEntry)
{
	lock
	IVTEntry *entry = Kernel::IVTable[ivtEntry];
	unlock
	return entry;
}*/