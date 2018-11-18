# About the project
A preemptive multithreaded kernel for Intel 8086 CPU with time sharing on a single CPU which supports context switching, event handling and semaphores using C++ and 8086 assembly language. 
There are two versions of this project, with and without system calls. System call is realized for every function in operating system kernel (classes Thread, Semaphore and Event).
Memory model is huge.
This was a final project in Operating Systems 1 course at School of Electrical Engineering, University of Belgrade. 


### How to use OS kernel:

 - Implement function userMain(argc, argv) in your user code, this is the main entry point for your application, arguments are passed from command line.

 - Create other user threads by extending OS class thread. You can specify thread name, thread stack size and time slice (minimum run time before thread can be interrupted). Implement custom thread code inside run method. Start thread by calling start method, thread will be started and run will be called on thread selection that depends on scheduler. 

 ### API:

 Threads:
```C++
 	virtual void run() {}
 	void start();
	void waitToComplete();
	static void sleep(Time timeToSleep); 

	ID getId();
	static Thread * getThreadById(ID id);
```
Semaphores:
 ```C++
	Semaphore(int init = 1); // makes new instance
    
    // If toBlock differs from 0 function returns 1 if thread is blocked and 0 otherwise.
    // If toBlock equals to 0 thread will not block and if value of semaphore is greater than 0 function returns 0, otherwise -1.
	virtual int wait(int toBlock); 
      
    virtual void signal(); // release first blocked thread on this semaphore

	int val () const;  // returns the current value of the semaphore
```
Events:
```C
	PREPAREENTRY(ivtNo, callOldISR) // adds new interrupt service routine that signals event and optionally calls old ISR. Routine is added inside system interrupt vector table, on specified entry number, ivtNo)

	Event(IVTNo ivtNo); // after calling prepareentry, create event instance
	int wait(); // blocks running thread, thread will be unblocked when any interrupt is raised on ivtNo
```