/*
 * semaphor.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernsem.h"
#include "global.h"

Semaphore::Semaphore(int init)
{
	lock
	myImpl = new KernelSem(init);
	unlock
}


Semaphore::~Semaphore()
{
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait(int toBlock)
{
	lock
	int ret = myImpl->wait(toBlock);
	unlock
	return ret;
}

void Semaphore::signal()
{
	lock
	myImpl->signal();
	unlock
}

int Semaphore::val() const
{
	lock
	int val = myImpl->value;
	unlock
	return val;
}
