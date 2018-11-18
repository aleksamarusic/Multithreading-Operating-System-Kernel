#ifndef _EVENT_H_
#define _EVENT_H_

#include "IVTEntry.h"


#define PREPAREENTRY(numEntry, callOld) \
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) { \
	if (callOld == 1) \
		newEntry##numEntry.callOldRoutine(); \
	newEntry##numEntry.signal(); \
}

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
	ID getID() { return myID; }
protected:
	friend class KernelEv;
	void signal();
private:
	ID myID;
};

#endif




