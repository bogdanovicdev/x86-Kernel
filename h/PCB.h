#ifndef _PCB_H_
#define _PCB_H_
#include "THREAD.H"
#include "ALLTHR.H"
const StackSize MAXSTACK = 65536;
typedef unsigned long StackSize;
const int MAXSIG=16;
const int MAXNUMSIG=100;
extern volatile  unsigned* tempstack;
extern volatile int lockFlag;


typedef enum thrstate{CREATED=1,READY,NOTFORSCHEDULER,OVER,MAIN}ThreadState;

struct SignalRequest {
public:
	int number;
	int completed;

	SignalRequest() {
		number=-1;
		completed=0;

	}


};

struct FuncElem {
	SignalHandler hand;
	FuncElem* next;

public:
	FuncElem(SignalHandler chand) {
		hand=chand;
		next=0;
	}


};
class PCB {
public:
	static void Handler0();
	friend class Thread;
	friend class BlockL;
	friend class TimerL;
	friend class ALLTHR;
	static ALLTHR* allThreads;
	volatile static int globallyBlocked[MAXSIG];
	volatile static PCB *running;
	static ID poslID;
	volatile ThreadState state;
	volatile int count;
	Thread* parent;
	SignalRequest sig[MAXNUMSIG];
	int locallyBlocked[MAXSIG];
	int numberRequested;
	SignalHandler hand;
	FuncElem* niz[MAXSIG];
	PCB(Thread *myThr,StackSize stSize,TIMESLICE timeSl);
	PCB(StackSize stSize,TIMESLICE timeSl);
	void waitToComplete();
	static void wrapper();
	~PCB();
	ID getID();
	static Thread* getThreadwithID(ID id);
	ID runningID();

	static void blockSignalGlobally(SignalId signal);

	static void unblockSignalGlobally(SignalId signal);

	void blockSignal(SignalId signal);

	void unblockSignal(SignalId signal);


	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);

	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);



	unsigned stackSize;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	ID id;
	BlockL* blist;
	Thread* myThread;
	unsigned timeSlice;
	int isSignaled;
};


























#endif _PCB_H_
