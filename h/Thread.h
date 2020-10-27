#include <iostream.h>

#ifndef _THREAD_H_
#define _THREAD_H_


typedef unsigned long StackSize;
typedef unsigned long ID;
typedef unsigned TIMESLICE;
typedef void (*SignalHandler) ();
typedef unsigned SignalId;

const StackSize defaultStackSize = 4096;
const TIMESLICE defaultTimeSlice=2;//x55ms

void dispatch();
class Semaphore;
class Thread {
protected:
	friend class PCB;
	friend class Scheduler;
	static ID poslID;
	friend class Semaphore;



protected:
	virtual void run(){};


public:

	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	void signal(SignalId signal);
	void registerHandler(SignalId signal,SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id,SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);


	PCB* myPCB;
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getThreadID();
	static Thread* getThreadById(ID id);
	static ID getRunningId();


	Thread(StackSize stSize=defaultStackSize,TIMESLICE timeSl=defaultTimeSlice);

};






























#endif
