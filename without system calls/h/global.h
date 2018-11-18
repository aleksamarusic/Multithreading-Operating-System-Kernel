#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define lock asm { pushf; cli; }

#define unlock  asm popf;

void tick();

typedef unsigned long StackSize;
typedef void interrupt (*InterruptRoutinePointer)(...);
typedef unsigned int Time;
typedef int ID;

#endif
