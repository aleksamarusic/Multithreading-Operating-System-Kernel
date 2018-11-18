#ifndef _LIST_H_
#define _LIST_H_

#include "pcb.h"

class List{
private:
	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* p);
	};

	Elem* head, *tail;
	int number;

public:

	List();
	~List();
	int numberOfElements() const { return number; }
	void add(PCB* );
	void removeFromList(PCB* );
	PCB* getFirst();
	Elem* getFirstElement() { return head; }
	int isEmpty();
	PCB * getThreadById(ID id);
};

#endif
