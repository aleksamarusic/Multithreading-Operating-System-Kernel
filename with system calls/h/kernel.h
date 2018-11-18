#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdio.h>
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>



#define sysCallPrepare(param) \
	int a = FP_SEG(param); \
	int b = FP_OFF(param); \
	asm { \
		mov ax, a; \
		mov bx, b; \
	} \
	lock \
	KernelThread::systemCallStart(); \
	unlock

extern volatile unsigned int preemptionDisabled;
extern volatile unsigned int explicitDispatchRequest;
/*
#define lock(lockFlag) \
			if(lockFlag) \
				++preemptionDisabled; \
			else asm { pushf; cli; }

#define unlock(lockFlag) \
			if(lockFlag){ \
				--preemptionDisabled; \
				if (!preemptionDisabled && explicitDispatchRequest) \
					dispatch(); \
			} \
			else asm popf;
*/

#define lock \
		asm { \
			pushf; \
			cli; \
		}

#define unlock \
		asm popf;

void tick();

typedef unsigned long StackSize;
typedef void interrupt (*InterruptRoutinePointer)(...);
typedef unsigned int Time;
typedef int ID;
typedef unsigned char IVTNo;
typedef enum SysCallType
{
	SYSTEM_DISPATCH,
	THREAD_CONSTRUCTOR, THREAD_DESTRUCTOR, THREAD_START, 
	THREAD_WAIT_TO_COMPLETE, THREAD_FINISH_THREAD, THREAD_SLEEP,
	SEMAPHORE_CONSTRUCTOR, SEMAPHORE_DESTRUCTOR, 
	SEMAPHORE_WAIT, SEMAPHORE_SIGNAL, SEMAPHORE_VAL,
	EVENT_CONSTRUCTOR, EVENT_DESTRUCTOR, EVENT_WAIT, EVENT_SIGNAL
} SystemCallType;




struct SystemCallParams
{
	SystemCallType systemCallType;
	ID objectID;
	unsigned long params[3];
	unsigned long returnValue;
};



template<class T>
class List;
class SleepList;
class Thread;
class IdleThread;
class PCB;
class KernelSem;
class KernelEv;
class KernelThread;
class IVTEntry;

class Kernel {
public:
	static void setKernelThread(KernelThread* kernelThr);
	static KernelThread * getKernelThread() { return runningKernelThread; }
	static void removeKernelThread() { runningKernelThread = 0; }

    static void setIdleThread(IdleThread* idle);
	static IdleThread * getIdleThread() { return idleThread; }
	static void removeIdleThread() { idleThread = 0; }

	static void setMainThread(Thread* mainThr);
	static Thread * getMainThread() { return mainThread; }
	static void removeMainThread() { mainThread = 0; }


	static void setRunningThreadPCB(PCB* running) { runningPCB = running; }
	static PCB * getRunningThreadPCB() { return runningPCB; }

    
    static PCB * getKernelThreadPCB() { return runningKernelThreadPCB; }
    static PCB * getIdleThreadPCB() { return idleThreadPCB; }
    static PCB * getMainThreadPCB() { return mainThreadPCB; }

	
	static void initAll();
    static void unInitAll();
    static void updateSleepWaitList();
    static void systemCallPrepare(SystemCallParams * );

	static unsigned explicitDispatchRequest;
    static unsigned preemptionDisabled;
	static unsigned inSystemCall;
	//static unsigned kernelThreadState;
    static KernelEv* eventsIVT[256];
	static IVTEntry* IVTable[256];
	static int callTimer;
	static int callIVT[256];


	static void timerInit();
	static void timerRestore();
	static void resetCounter();

	static Time timeCounter;
	static InterruptRoutinePointer oldRoutine;	
	static void interrupt timer(...);
        
    

private:
	friend class KernelThread;
	static List<PCB> *allPCBs;
    static List<KernelSem> *allKernelSemaphors;
    static List<KernelEv> *allKernelEvents;
	static SleepList *blockedOnSleep;

    static PCB *mainThreadPCB;
	static PCB *runningPCB;
	static PCB* idleThreadPCB;
	static PCB* runningKernelThreadPCB;

    static IdleThread* idleThread;
    static Thread * mainThread;
    static KernelThread* runningKernelThread;

	static void sleep(Time timeToSleep);
};


#endif
