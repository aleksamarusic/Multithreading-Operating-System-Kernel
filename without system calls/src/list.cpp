#include "list.h"
#include "pcb.h"
#include <dos.h>

List::Elem::Elem(PCB* p): pcb(p), next(0) {}

List::List(): head(0), tail(0), number(0) {}

List::~List(){

lock
	while(head){
		tail = head;
		head = head->next;
		delete tail;
	}
	tail = 0;
unlock
}

void List::add(PCB* p){

lock
	Elem* elem = new Elem(p);
	if(head == 0) head = elem;
	else tail->next = elem;
	tail = elem;
	number++;
unlock
}

void List::removeFromList(PCB* p){

lock
	Elem* tmp = head, *prev = 0;
	while(tmp){
		if(p == tmp->pcb) break;
		prev = tmp;
		tmp = tmp->next;
	}
	if(tmp != 0){
		if(prev == 0){
			head = head->next;
			delete tmp;
			if(head == 0) tail = 0;
		}
		else if(tmp->next == 0){
			tail = prev;
			delete tmp;
			tail->next = 0;
		}
		else {
			prev->next = tmp->next;
			delete tmp;
		}
	}
unlock
}

PCB * List::getThreadById(ID id)
{
	Elem * curr = head;
	while (curr && (curr->pcb->getID() != id))
		curr = curr->next;
	if (curr)
		return curr->pcb;
	return 0;
}

PCB* List::getFirst(){

	lock
	if(head == 0){
		unlock
		return 0;
	}
	Elem* tmp = head;
	if(head->next) head = head->next;
	else head = tail = 0;
	PCB* p = tmp->pcb;
	delete tmp;
	number--;
	unlock
	return p;
}


int List::isEmpty(){
	lock
	int value = 0;
	if(head == 0) value = 1;
	unlock
	return value;
}

