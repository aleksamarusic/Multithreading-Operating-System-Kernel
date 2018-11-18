//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    bounded.cpp
//   Date:    Jun 2018
//===========================================================================//
#include "bounded.h"

#include <stdlib.h>

#include "intLock.h"

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}
	
BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	spaceAvailable.wait(1);
	mutexa.wait(1);
		buffer[tail] = d;
		tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	return 0;
}

char BoundedBuffer::take () {
	itemAvailable.wait(1);
	mutext.wait(1);
		char d = buffer[head];
		head = (head+1)%Size;
	mutext.signal();
	spaceAvailable.signal();
	return d;
}
