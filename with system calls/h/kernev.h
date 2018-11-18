#ifndef _KERNEV_H_
#define _KERNEV_H_

#include "kernel.h"

class PCB;

class KernelEv
{
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
	ID getID() { return id; }

private:
	ID id;
	PCB *eventOwner;
	int value;
	IVTNo IVTNumber;
	int blocked;

	static ID kernelEvID;
};

#endif

