#include "semaphor.h"
#include "kernsem.h"


Semaphore::Semaphore(int init)
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SEMAPHORE_CONSTRUCTOR;
	systemCallParams.params[0] = init;

	Kernel::systemCallPrepare(&systemCallParams);

	myID = systemCallParams.returnValue;
	 	
	unlock
}


Semaphore::~Semaphore()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SEMAPHORE_DESTRUCTOR;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

int Semaphore::wait(int toBlock)
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SEMAPHORE_WAIT;
	systemCallParams.params[0] = myID;
	systemCallParams.params[0] = toBlock;

	Kernel::systemCallPrepare(&systemCallParams);

	int ret = systemCallParams.returnValue;
	 	
	unlock
	return ret;
}

void Semaphore::signal()
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SEMAPHORE_SIGNAL;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);
	 	
	unlock
}

int Semaphore::val() const
{
	lock
	SystemCallParams systemCallParams;
	systemCallParams.systemCallType = SEMAPHORE_VAL;
	systemCallParams.params[0] = myID;

	Kernel::systemCallPrepare(&systemCallParams);

	int ret = systemCallParams.returnValue;
	 	
	unlock
	return ret;
}
