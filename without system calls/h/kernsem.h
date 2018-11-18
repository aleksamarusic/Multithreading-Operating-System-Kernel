#ifndef _KERNSEM_H
#define _KERNSEM_H

class List;

class KernelSem
{
public:
	KernelSem(int init);
	~KernelSem();
	int wait(int toBlock);
	void signal();
	int value;
	List *list;
};

#endif