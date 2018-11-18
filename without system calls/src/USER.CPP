//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    user.cpp


//   Date:    Jun 2018
//===========================================================================//
#include <iostream.h>
#include <stdlib.h>

#include <event.h>
#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"
#include "user.h"

//---------------------------------------------------------------------------//
//  Ovo se menja u testu
//---------------------------------------------------------------------------//

Time TIME_SLICE = 2;   // 0 ili defaultTimeSlice

int N = 3;            // 1 <= N <= 19

//---------------------------------------------------------------------------//

volatile int theEnd=0;

class Producer : public Thread {
public:
	Producer (BoundedBuffer* bb, char y, Time time_slice);
	virtual ~Producer() {waitToComplete(); }
protected:
	virtual void run ();
	char produce() {return x;}; // Produce an item

private:
	BoundedBuffer* myBuffer;
	char x;
};



//---------------------------------------------------------------------------//
class Consumer : public Thread {
public:
	Consumer (BoundedBuffer* bb) : Thread(defaultStackSize, 0), myBuffer(bb) {}
	virtual ~Consumer() {waitToComplete(); }

protected:
	virtual void run ();
	void consume(char p); // Consume an item

private:
	BoundedBuffer* myBuffer;
};




//---------------------------------------------------------------------------//
// Korisnicki program mora obavezno da definise ovu f-ju
//---------------------------------------------------------------------------//
void tick(){
}

//---------------------------------------------------------------------------//


Producer::Producer (BoundedBuffer* bb, char y, Time time_slice) 
: Thread(defaultStackSize, time_slice),myBuffer(bb), x(y) {}
	
	void Producer::run () {
	while(!theEnd) {
		char d = produce();
		myBuffer->append(d);
		Thread::sleep(10);
	}
}

//---------------------------------------------------------------------------//


void Consumer::consume(char p) {
	intLock
	cout<<p<<" ";
	intUnlock
} // Consume an item

void Consumer::run () {
		
	int i = 0;
	while(!theEnd) {
		char d = myBuffer->take();
		consume(d);
		if (i++ == 40) {
			Thread::sleep(5);
			i = 0;
		}else for(int j=0;j<200;j++);
	}
 
	intLock
	cout<<endl<<"ESC pressed - empty the buffer!"<<endl;
	intUnlock
	
	while (myBuffer->fullCount()){
		char d = myBuffer->take();
		consume(d);
		dispatch();
	}
	
	
	intLock
	cout<<endl<<"Happy End"<<endl;
	intUnlock
}





int userMain (int argc, char* argv[])
{

	lock
	cout<<"USAO U USERMAIN!"<<endl;
	unlock

	BoundedBuffer *buff;
	Consumer *con;
	
	intLock
	if(argc <2){
		cout<<"Invalid input!"<<endl;
		intUnlock
		return -1;
	}
	int buffSize = atoi(argv[1]);
	N = atoi(argv[2]);
	N = N>19 ? 19 : N;
	TIME_SLICE = atoi(argv[3]);
	
	if(buffSize<N) {
		cout<<"Number of Produsers is larger then Buffer size!"<<endl;
		intUnlock
		return 1;
	}

	buff = new BoundedBuffer(buffSize);
	Producer **pro = new Producer*[N];
	KeyboardEvent* kev;
	int i;
	
	lock
	cout<<"KREIRAO BUFFER I NIZ PRODUCERA"<<endl;
	unlock

	con = new Consumer(buff);
	



	con->start();

	lock
	cout<<"KREIRAO I STARTOVAO CONSUMERA"<<endl;
	unlock

	for (i=0; i<N; i++){
		pro[i] = new Producer(buff,'0'+i, TIME_SLICE);
		pro[i]->start();
	}
  	

	lock
	cout<<"KREIRAO I STARTOVAO PRODUCERE"<<endl;
	unlock

	kev = new KeyboardEvent(buff);
	intUnlock
	
	lock
	cout<<"KREIRAO KeyboardEvent"<<endl;
	unlock

	kev->start();
	
	for (i=0; i<N; i++){
		delete pro[i];
	}

	lock
	cout<<"OBRISAO PRODUCERE"<<endl;
	unlock

	delete [] pro;
	delete kev;
	delete con;
	delete buff;

	intLock
	cout<<"userMain finished!"<<endl;
	intUnlock
	return 0;
}
//---------------------------------------------------------------------------//


