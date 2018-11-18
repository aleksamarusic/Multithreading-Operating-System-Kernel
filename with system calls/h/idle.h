#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"

volatile unsigned a = 0;

class IdleThread: public Thread
{
public:
	IdleThread () : Thread (512, 1) {}
	
	void run ()
	{
		while(a == 0);
	}

};
#endif



