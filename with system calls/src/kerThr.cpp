#include "kerThr.h"
#include "pcb.h"
#include "schedule.h"
#include "kernsem.h"
#include "kernev.h"
#include "list.h"
#include <iostream.h>


KernelThread *KernelThread::instance = 0;
SystemCallParams * KernelThread::systemCallParams = 0;



void interrupt KernelThread::systemCallStart(...)
{
	static unsigned int rax;
	static unsigned int rbx;
	asm {
		mov rax, ax;
		mov rbx, bx;
	}
	systemCallParams = (SystemCallParams*)MK_FP(rax, rbx);

	static unsigned int tss;
	static unsigned int tsp;
	static unsigned int tbp;
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}

	Kernel::getRunningThreadPCB()->ss = tss;
	Kernel::getRunningThreadPCB()->sp = tsp;
	Kernel::getRunningThreadPCB()->bp = tbp;

	tss = Kernel::getKernelThreadPCB()->ss;
	tsp = Kernel::getKernelThreadPCB()->sp;
	tbp = Kernel::getKernelThreadPCB()->bp;
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
	Kernel::inSystemCall = 1;
	//Kernel::kernelThreadState = 1;
}

void KernelThread::run()
{
	while(1)
	{
		if(Kernel::inSystemCall == 1)
		{
			switch (systemCallParams->systemCallType)
			{
			case THREAD_CONSTRUCTOR:
			{
				PCB* pcb = new PCB(systemCallParams->params[0], systemCallParams->params[1]);
				pcb->initializeStack((Thread*)systemCallParams->params[2]);
				Kernel::allPCBs->add(pcb);
				systemCallParams->returnValue = pcb->getID();
			} break;
			case THREAD_DESTRUCTOR:
			{
				PCB* pcb = Kernel::allPCBs->getByID(systemCallParams->params[0]);
				Kernel::allPCBs->removeFromList(pcb);
				delete pcb;
			} break;
			case THREAD_START:
			{
				PCB* pcb = Kernel::allPCBs->getByID(systemCallParams->params[0]);
				pcb->start();
			} break;
			case THREAD_WAIT_TO_COMPLETE:
			{
				PCB* pcb = Kernel::allPCBs->getByID(systemCallParams->params[0]);
				pcb->waitToComplete();
			} break;
			case THREAD_FINISH_THREAD:
			{
				PCB* pcb = Kernel::allPCBs->getByID(systemCallParams->params[0]);
				pcb->finishThread();
			} break;
			case THREAD_SLEEP:
			{
				//PCB* pcb = Kernel::allPCBs->getByID(systemCallParams->params[0]);
				Time tTS = systemCallParams->params[0];
				if(tTS > 0)
					Kernel::sleep(tTS);
			} break;
			case SEMAPHORE_CONSTRUCTOR:
			{
				KernelSem* kernelSem = new KernelSem(systemCallParams->params[0]);
				Kernel::allKernelSemaphors->add(kernelSem);
				systemCallParams->returnValue = kernelSem->getID();
			} break;
			case SEMAPHORE_DESTRUCTOR:
			{
				KernelSem* kernelSem = Kernel::allKernelSemaphors->getByID(systemCallParams->params[0]);
				Kernel::allKernelSemaphors->removeFromList(kernelSem);
				delete kernelSem;
			} break;
			case SEMAPHORE_WAIT:
			{
				KernelSem* kernelSem = Kernel::allKernelSemaphors->getByID(systemCallParams->params[0]);
				kernelSem->wait(systemCallParams->params[1]);
			} break;
			case SEMAPHORE_SIGNAL:
			{
				KernelSem* kernelSem = Kernel::allKernelSemaphors->getByID(systemCallParams->params[0]);
				kernelSem->signal();
			} break;
			case SEMAPHORE_VAL:
			{
				KernelSem* kernelSem = Kernel::allKernelSemaphors->getByID(systemCallParams->params[0]);
				systemCallParams->returnValue = kernelSem->val();
			} break;
			case EVENT_CONSTRUCTOR:
			{
				KernelEv* kernelEvent = new KernelEv((IVTNo)systemCallParams->params[0]);
				Kernel::allKernelEvents->add(kernelEvent);
				systemCallParams->returnValue = kernelEvent->getID();
			} break;
			case EVENT_DESTRUCTOR:
			{
				KernelEv* kernelEvent = Kernel::allKernelEvents->getByID(systemCallParams->params[0]);
				Kernel::allKernelEvents->removeFromList(kernelEvent);
				delete kernelEvent;
			} break;
			case EVENT_WAIT:
			{
				KernelEv* kernelEvent = Kernel::allKernelEvents->getByID(systemCallParams->params[0]);
				kernelEvent->wait();
			} break;
			case EVENT_SIGNAL:
			{
				KernelEv* kernelEvent = Kernel::allKernelEvents->getByID(systemCallParams->params[0]);
				kernelEvent->signal();
			} break;
			case SYSTEM_DISPATCH:
			{
				if(Kernel::getRunningThreadPCB()->getState() == PCB::READY && Kernel::getRunningThreadPCB() != Kernel::getIdleThreadPCB())
					Scheduler::put(Kernel::getRunningThreadPCB());
				Kernel::setRunningThreadPCB(Scheduler::get());
				if(!Kernel::getRunningThreadPCB())
					Kernel::setRunningThreadPCB(Kernel::getIdleThreadPCB());
				Kernel::timeCounter = Kernel::getRunningThreadPCB()->timeSlice;
			} break;
			}
			lock
			systemCallEnd();
			unlock
		}
	}
}


void interrupt KernelThread::systemCallEnd(...)
{
	static unsigned int tss;
	static unsigned int tsp;
	static unsigned int tbp;

	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}

	Kernel::getKernelThreadPCB()->ss = tss;
	Kernel::getKernelThreadPCB()->sp = tsp;
	Kernel::getKernelThreadPCB()->bp = tbp;

	tss = Kernel::getRunningThreadPCB()->ss;
	tsp = Kernel::getRunningThreadPCB()->sp;
	tbp = Kernel::getRunningThreadPCB()->bp;

	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}

	Kernel::inSystemCall = 0;
	IVTSignalAll();
}

void KernelThread::IVTSignalAll()
{
	if (Kernel::callTimer)
		Kernel::timer();
	for (int i = 0; i < 256; i++)
	{
		if (Kernel::callIVT[i])
		{
			Kernel::callIVT[i] = 0;
			IVTEntry* ivtEntry = Kernel::IVTable[i];
			if (ivtEntry)
				ivtEntry->signal();
		}
	}
}

KernelThread::KernelThread() : Thread(4096, 0)
{
	setvect(0x61, systemCallStart);
	setvect(0x63, systemCallEnd);
}

KernelThread* KernelThread::getInstance()
{
	if (instance == 0)
		instance = new KernelThread();
	return instance;
}

KernelThread::~KernelThread()
{
	instance = 0;
	if (systemCallParams)
		delete systemCallParams;
	systemCallParams = 0;
}
