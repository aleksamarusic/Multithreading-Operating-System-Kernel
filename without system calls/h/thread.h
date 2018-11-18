#ifndef _THREAD_H_
#define _THREAD_H_

#include "global.h"


const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 65536;  //64KB
const Time defaultTimeSlice = 2; // default = 2*55ms

class PCB;

class Thread
{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	
	static void sleep(Time timeToSleep);

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	PCB *myPCB;
	friend class PCB;
	friend int main(int argc, char* argv[]);
	Thread(StackSize stackSize=defaultStackSize, Time timeSlice=defaultTimeSlice);
	virtual void run() {}
private:
	
};

void dispatch();

#endif
