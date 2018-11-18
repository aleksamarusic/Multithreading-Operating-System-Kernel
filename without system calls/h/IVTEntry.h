#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

typedef void interrupt (*InterruptRoutinePointer)(...);

typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry
{
public:
	IVTEntry (IVTNo, InterruptRoutinePointer);
	~IVTEntry();

	void callOldRoutine();
	void signal();
	static IVTEntry * getIVTEntry (IVTNo);
	void setKernelEvent(KernelEv *);
	void removeKernelEvent();

private:
	IVTNo myEntry;
	KernelEv *myKernelEvent;
	InterruptRoutinePointer oldRoutine;
	static IVTEntry *IVTable[];
	friend class KernelEv;
};

#endif

