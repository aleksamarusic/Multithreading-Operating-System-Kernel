/*
 * IVTEntry.cpp
 *
 *  Created on: May 3, 2017
 *      Author: OS1
 */

#include "IVTEntry.h"
#ifndef BCC_BLOCK_IGNORE
#include <dos.h>
#endif
#include "KernEv.h"
#include "global.h"


IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo entry, InterruptRoutinePointer newInterruptRoutine)
{
	lock
	myEntry = entry;
	oldRoutine = getvect(entry);
	setvect(entry, newInterruptRoutine);
	myKernelEvent = 0;
	IVTable[entry] = this;
	unlock
}

IVTEntry::~IVTEntry()
{
	lock
	IVTable[myEntry] = 0;
	myKernelEvent = 0;
	setvect(myEntry, oldRoutine);
	callOldRoutine();
	unlock
}

void IVTEntry::setKernelEvent(KernelEv* event)
{
	lock
	myKernelEvent = event;
	unlock
}

void IVTEntry::removeKernelEvent()
{
	lock
	myKernelEvent = 0;
	unlock
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
	lock
	if(myKernelEvent != 0) 
		myKernelEvent->signal();
	unlock
}

IVTEntry * IVTEntry::getIVTEntry(IVTNo ivtEntry)
{
	lock
	IVTEntry *entry = IVTable[ivtEntry];
	unlock
	return entry;
}