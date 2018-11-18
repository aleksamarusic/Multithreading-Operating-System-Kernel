#include "pcb.h"
#include <iostream.h>
#include "timer.h"
#include "idle.h"
#include <stdlib.h>

int userMain(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	cout<<"main"<<endl;
	PCB::setMainThread(new Thread(1024, 2));
	PCB::getMainThread()->setState(PCB::READY);
	PCB::setRunningThread(PCB::getMainThread());
	PCB::setIdleThread((Thread*)(new Idle()));
	PCB::getIdleThread()->start();
	Timer::timerInit();
	int ret = userMain(argc, argv);
	Timer::timerRestore();
	PCB::removeMainThread();
	PCB::removeIdleThread();
	return 0;
}


