#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "list.h"

class PCB
{
public:
	typedef enum State { CREATED, READY, BLOCKED, FINISHED };
	PCB (StackSize stackSize, Time timeSlice);
	~PCB ();
	void start();
	//static void wrapper();
	void waitToComplete();
	
	
	
	ID getID() { return id; };
	//static PCB *getPcbById(ID);
	//Thread* getMyThread() const { return myThread; }

	void setState(State state) { this->state = state; }
	State getState() { return state; }
	
	unsigned  ss,sp,bp;
	Time timeSlice;
	
protected:
	State state;
	StackSize stackSize;
	unsigned *stack;
	static ID pcbID;
	ID id;
	List<PCB> *blockedOnThread;
	void finishThread();
	void initializeStack(Thread * thread);
	
	friend class KernelThread;
	//friend void interrupt Timer::timer(...);

};




#endif
