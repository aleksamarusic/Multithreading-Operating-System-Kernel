#include "sleepLst.h"
#include "pcb.h"
#include "schedule.h"

SleepList::SleepList(): head(0), tail(0), maxTime(0), numberOfElements(0) {}

SleepList::~SleepList()
{
	while (head != 0)
		decrease();
}

void SleepList::insert(PCB* pcb, Time time)
{
	Element* elem = new Element(pcb, time);
	Element* cur = head;
	Element* prev = 0;
	while (cur && cur->time <= elem->time)
	{
		elem->time -= cur->time;
		prev = cur;
		cur = cur->next;
	}
	if (cur)
		cur->time -= elem->time;
	else
		maxTime = time;
	if (prev)
		prev->next = elem;
	else
		head = elem;
	elem->next = cur;
	numberOfElements++;
}

void SleepList::decrease()
{
	if (head)
	{
		if (head->time > 0)
			head->time--;
		while (head && head->time == 0)
		{
			Element* toDelete = head;
			head = head->next;
			toDelete->pcb->setState(PCB::READY);
			Scheduler::put(toDelete->pcb);
			delete toDelete;
			numberOfElements--;
		}
	}
}

