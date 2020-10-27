#include "THREAD.H"
#include "PCB.H"
#include "SCHEDULE.H"
#include "SETUP.H"
#include "SEMAPHOR.H"
#include <iostream.h>

volatile int context_on_demand=0;


#include "PCB.H"
class Semaphore;
Thread::Thread(StackSize stSize,TIMESLICE timeSl) {

	myPCB=new PCB(this,stSize,timeSl);


}

void Thread::start()
{

asm pushf;
	asm cli;

	if(this->myPCB->state==CREATED) this->myPCB->state=READY;
	Scheduler::put(myPCB);
	asm popf;
}


void Thread::waitToComplete() {

	myPCB->waitToComplete();

	}

void Thread::blockSignalGlobally(SignalId signal) {
	asm pushf
	asm cli
	PCB::blockSignalGlobally(signal);
	asm popf


}

void Thread::unblockSignalGlobally(SignalId signal) {
	asm pushf
	asm cli
	PCB::unblockSignalGlobally(signal);
	asm popf
}

void Thread::blockSignal(SignalId signal) {
	asm pushf
	asm cli
	myPCB->blockSignal(signal);
	asm popf
}

void Thread::unblockSignal(SignalId signal) {
	asm pushf
	asm cli
	myPCB->unblockSignal(signal);
	asm popf

}


void Thread::signal(SignalId signal) {

	asm pushf
	asm cli
	myPCB->signal(signal);
	asm popf

}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	asm pushf
	asm cli
	myPCB->registerHandler(signal,handler);
	asm popf


}

void Thread::unregisterAllHandlers(SignalId id) {

	asm pushf
	asm cli
	myPCB->unregisterAllHandlers(id);
	asm popf
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {

	asm pushf
	asm cli
	myPCB->swap(id,hand1,hand2);
	asm popf
}




Thread::~Thread()
{

	delete myPCB;

}

Thread * Thread::getThreadById(ID id)
{
	return PCB::getThreadwithID(id);
}

ID Thread::getThreadID()
{
	return this->myPCB->getID();
}



ID Thread::getRunningId()
{
	return PCB::running->id; //scope of the variable
}



void dispatch(){

	asm cli;
		context_on_demand=1;
		//cout<<"pozvan dispatch()\n";
		asm int 8h
		asm sti;


}





