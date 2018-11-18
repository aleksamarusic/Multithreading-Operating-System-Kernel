#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"

class List;
class SleepList;

class PCB
{
public:
	static enum State { CREATED, READY, BLOCKED, FINISHED };
	PCB (StackSize stackSize, Time timeSlice, Thread *thread);
	~PCB ();
	void start();
	static void wrapper();
	static void sleep(Time timeToSleep);
	void waitToComplete();
	
	
	ID getID() { return id; };
	static PCB *getPcbById(ID);
	Thread* getMyThread() const { return myThread; }

	void setState(State state) { this->state = state; }
	State getState() { return state; }

	static void setIdleThread(Thread* idle) { idlePCB = idle->myPCB; }
	static PCB * getIdleThread() { return idlePCB; }
	static void removeIdleThread() { delete idlePCB->getMyThread(); }

	static void setMainThread(Thread* mainThr) { mainThreadPCB = mainThr->myPCB; }
	static PCB * getMainThread() { return mainThreadPCB; }
	static void removeMainThread() { delete mainThreadPCB->getMyThread(); }

	static void setRunningThread(PCB* runningPCB) { running = runningPCB; }
	static PCB * getRunningThread() { return running; }

	static void updateSleepWaitList();

	static volatile unsigned explicitDispatchRequest;
	unsigned  ss,sp,bp;
	Time timeSlice;
	
protected:
	State state;
	
	
	StackSize stackSize;
	Thread *myThread;
	unsigned *stack;
	
	static ID pcbID;
	ID id;
	List *blockedOnThread;
	static PCB *mainThreadPCB;
	static PCB *running;
	static PCB* idlePCB;
	static List *allPCBs;
	static SleepList *blockedOnSleep;
	

	void initializeStack();
	void finishThread();
	//friend void interrupt Timer::timer(...);

};




#endif
