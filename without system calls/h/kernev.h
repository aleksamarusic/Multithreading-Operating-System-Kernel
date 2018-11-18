#ifndef _KERNEV_H_
#define _KERNEV_H_

typedef unsigned char IVTNo;

class PCB;

class KernelEv
{
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	PCB *eventOwner;
	int value;
	IVTNo ivtNo;
	int blocked;
};

#endif

