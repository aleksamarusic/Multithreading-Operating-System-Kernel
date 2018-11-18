#ifndef _LIST_H_
#define _LIST_H_

#include "kernel.h"

template<class T>
class List{
private:
	struct Elem{
		T* t;
		Elem* next;
		Elem(T* p): t(p), next(0) {}
	};

	Elem* head, *tail;
	int number;

public:

	List(): head(0), tail(0), number(0) {}
	~List();
	int numberOfElements() const { return number; }
	void add(T* );
	void removeFromList(T* );
	T* getFirst();
	int isEmpty();
	T * getByID(ID id);
};

//List::Elem::Elem(T* p): t(p), next(0) {}

//List::List(): head(0), tail(0), number(0) {}

template<class T>
List<T>::~List(){

	
	while(head){
		tail = head;
		head = head->next;
		delete tail;
	}
	tail = 0;
	
}

template<class T>
void List<T>::add(T* p){

	
	Elem* elem = new Elem(p);
	if(head == 0) head = elem;
	else tail->next = elem;
	tail = elem;
	number++;
	
}

template<class T>
void List<T>::removeFromList(T* p){

	
	Elem* tmp = head, *prev = 0;
	while(tmp){
		if(p == tmp->t) break;
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
	
}

template<class T>
T * List<T>::getByID(ID id)
{
	Elem * curr = head;
	while (curr && (curr->t->getID() != id))
		curr = curr->next;
	if (curr)
		return curr->t;
	return 0;
}

template<class T>
T* List<T>::getFirst(){

	
	if(head == 0){
		
		return 0;
	}
	Elem* tmp = head;
	if(head->next) head = head->next;
	else head = tail = 0;
	T* p = tmp->t;
	delete tmp;
	number--;
	
	return p;
}

template<class T>
int List<T>::isEmpty(){
	
	int value = 0;
	if(head == 0) value = 1;
	
	return value;
}


#endif
