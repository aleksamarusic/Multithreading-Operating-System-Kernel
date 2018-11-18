#ifndef _KERTHR_H_
#define _KERTHR_H_


#include "thread.h"

class KernelThread : public Thread
{
private:
	static KernelThread * instance;
	static SystemCallParams * systemCallParams;
	//unsigned 
	virtual void run();
	KernelThread();

public:
	
	static void interrupt systemCallStart(...);
	static void interrupt systemCallEnd(...);
	static void IVTSignalAll();
	static KernelThread* getInstance();

	
	
	~KernelThread();
};

#endif
