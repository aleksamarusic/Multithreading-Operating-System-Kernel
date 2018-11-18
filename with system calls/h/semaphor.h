#ifndef _SEMAPHOR_H_
#define _SEMAPHOR_H_

#include "kernel.h"

class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (int toBlock);
	virtual void signal();
	int val () const;
private:
	ID myID;
};

#endif
