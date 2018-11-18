#ifndef _KERNSEM_H
#define _KERNSEM_H

#include "list.h"
#include "kernel.h"

class PCB;

class KernelSem
{
public:
	KernelSem(int init);
	~KernelSem();
	int wait(int toBlock);
	void signal();
	int val() { return value; }
	ID getID() { return id; }

private:
	int value;
	ID id;
	static ID kernelSemID;
	List<PCB> *list;
};

#endif