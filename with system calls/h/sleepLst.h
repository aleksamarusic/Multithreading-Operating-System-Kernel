#ifndef _SLEEPLST_H_
#define _SLEEPLST_H_

#include "kernel.h"

class SleepList
{
private:
	class Element
	{
	public:
		PCB* pcb;
		Element* next;
		Time time;

		Element(PCB* pcb, unsigned time)
		{
			this->pcb = pcb;
			this->time = time;
			next = 0;
		}
	};

	Element* head;
	Element* tail;
	unsigned maxTime;
	unsigned numberOfElements;

public:
	SleepList();
	~SleepList();
	void insert(PCB* pcb, Time time);
	void decrease();
};

#endif
