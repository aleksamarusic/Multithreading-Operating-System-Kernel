#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "kernel.h"

class IVTEntry
{
public:
	IVTEntry (IVTNo, InterruptRoutinePointer);
	~IVTEntry();

	void callOldRoutine();
	void signal();
	//static IVTEntry * getIVTEntry (IVTNo);
	void setKernelEvent(KernelEv *);
	void removeKernelEvent();

private:
	IVTNo IVTNumber;
	//KernelEv *myKernelEvent;
	InterruptRoutinePointer oldRoutine;
	friend class KernelEv;
};

#endif

