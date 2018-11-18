#include "kernel.h"
#include "sleepLst.h"
#include "list.h"
#include "idle.h"
#include "thread.h"
#include "pcb.h"
#include "kerThr.h"
#include <iostream.h>


PCB *Kernel::mainThreadPCB = 0;
PCB *Kernel::runningPCB = 0;
PCB* Kernel::idleThreadPCB = 0;
PCB* Kernel::runningKernelThreadPCB = 0;
IdleThread* Kernel::idleThread = 0;
KernelThread* Kernel::runningKernelThread = 0;
Thread* Kernel::mainThread = 0;
List<PCB> *Kernel::allPCBs = new List<PCB>();
List<KernelSem> *Kernel::allKernelSemaphors = new List<KernelSem>();
List<KernelEv> *Kernel::allKernelEvents = new List<KernelEv>();
SleepList *Kernel::blockedOnSleep = new SleepList();
unsigned Kernel::explicitDispatchRequest = 0;
unsigned Kernel::preemptionDisabled = 0;
unsigned Kernel::inSystemCall = 0;
//unsigned Kernel::kernelThreadState = 0;
KernelEv* Kernel::eventsIVT[256] = { 0 };
IVTEntry* Kernel::IVTable[256] = { 0 };
int Kernel::callTimer = 0;
int Kernel::callIVT[256] = { 0 };


void Kernel::setKernelThread(KernelThread * kernelThr)
{ 
	runningKernelThread = kernelThr; 
	runningKernelThreadPCB = allPCBs->getByID(kernelThr->getID());
	//kernelThreadState = 0; 
}

void Kernel::setIdleThread(IdleThread* idle) 
{ 
	idleThread = idle; 
	idleThreadPCB = allPCBs->getByID(idle->getID()); 
}

void Kernel::setMainThread(Thread* mainThr) 
{ 
	mainThread = mainThr; 
	mainThreadPCB = allPCBs->getByID(mainThread->getID());
}


void Kernel::sleep(Time timeToSleep)
{
	Kernel::getRunningThreadPCB()->setState(PCB::BLOCKED);
	Kernel::blockedOnSleep->insert(Kernel::getRunningThreadPCB(), timeToSleep);
	dispatch();
}

void Kernel::updateSleepWaitList()
{
	blockedOnSleep->decrease();
}

void Kernel::systemCallPrepare(SystemCallParams * param)
{
	int a = FP_SEG(param);
	int b = FP_OFF(param);
	asm {
		mov ax, a;
		mov bx, b;
	}
	lock
	KernelThread::systemCallStart();
	unlock
}
void Kernel::initAll()
{
    cout<<"main"<<endl;
	Kernel::setKernelThread(KernelThread::getInstance());
	Kernel::setMainThread(new Thread(1024, 2));
	Kernel::getMainThreadPCB()->setState(PCB::READY);
	Kernel::setRunningThreadPCB(Kernel::getMainThreadPCB());
	IdleThread *idleThread = new IdleThread();
	Kernel::setIdleThread(idleThread);
	Kernel::getIdleThread()->start();
	Kernel::timerInit();
}

void Kernel::unInitAll()
{
    Kernel::timerRestore();
	Kernel::removeIdleThread();
	Kernel::removeMainThread();
	Kernel::removeKernelThread();
}





Time Kernel::timeCounter = 0;
InterruptRoutinePointer Kernel::oldRoutine = 0;


void Kernel::timerInit()
{
	oldRoutine = getvect(8);
	setvect(8, &timer);
	setvect(0x60, oldRoutine);
	resetCounter();
}

void Kernel::timerRestore()
{
	setvect(8, oldRoutine);
}

void Kernel::resetCounter()
{
	timeCounter = 0;
}

void interrupt Kernel::timer(...)
{
	if (Kernel::inSystemCall == 0)
	{
		oldRoutine();
		tick();
		Kernel::updateSleepWaitList();
		if(Kernel::getRunningThreadPCB()->timeSlice != 0) 
			timeCounter--;
		if(timeCounter > 0 || Kernel::getRunningThreadPCB()->timeSlice == 0)
			return;
		dispatch();
	}
	else
	{
		Kernel::callTimer = 1;
	}

	/*
	static volatile unsigned tsp, tss, tbp;
	if(!PCB::explicitDispatchRequest) 
	{
		oldRoutine();
		tick();
		PCB::updateSleepWaitList();
		if(PCB::getRunningThread()->timeSlice != 0) 
			timeCounter--;
		if(timeCounter > 0 || PCB::getRunningThread()->timeSlice == 0)
			return;
	}
	PCB::explicitDispatchRequest = 0;
	
	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}
	PCB::getRunningThread()->sp = tsp;
	PCB::getRunningThread()->ss = tss;
	PCB::getRunningThread()->bp = tbp;

	if(PCB::getRunningThread()->getState() == PCB::READY && PCB::getRunningThread() != PCB::getIdleThread())
		Scheduler::put(PCB::getRunningThread());
	PCB::setRunningThread(Scheduler::get());
	if(!PCB::getRunningThread())
		PCB::setRunningThread(PCB::getIdleThread());
	timeCounter = PCB::getRunningThread()->timeSlice;
	tsp = PCB::getRunningThread()->sp;
	tss = PCB::getRunningThread()->ss; 
	tbp = PCB::getRunningThread()->bp;
	
	timeCounter = PCB::getRunningThread()->timeSlice;

	asm {
		mov sp, tsp
		mov ss, tss
		mov bp, tbp
	}*/
}

