#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

volatile unsigned a = 0;

class Idle: public Thread
{
public:
	Idle () : Thread (512, 1) {}
	
	PCB * getPcb()
	{
		return myPCB;
	}
	
	void run ()
	{
		while(a == 0);
	}

};
#endif



