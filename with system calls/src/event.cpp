#include "event.h"
#include "kernel.h"


Event::Event(IVTNo ivtNo)
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = EVENT_CONSTRUCTOR;
	systemCallParams.params[0] = ivtNo;

	Kernel::systemCallPrepare(&systemCallParams);

	myID = systemCallParams.returnValue;
	 	
	unlock
}

Event::~Event()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = EVENT_DESTRUCTOR;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

void Event::wait()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = EVENT_WAIT;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

void Event::signal()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = EVENT_SIGNAL;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

