#include "PCB.H"
#include "BLOCKL.H"
#include "SCHEDULE.H"
#include "DOS.H"
#include <iostream.h>
#include "SETUP.H"
void dispatch();

ALLTHR* PCB::allThreads=new ALLTHR();

extern int context_on_demand;
volatile PCB* PCB::running=0;
ID PCB::poslID=0;
volatile int PCB::globallyBlocked[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
extern volatile int runFlag;


void PCB::Handler0() {
	asm pushf
	asm cli
	PCB* p=PCB::running->blist->cutoff();



		while(p!=0) {

			p->state=READY;
			Scheduler::put(p);
			p=PCB::running->blist->cutoff();
		}
		((PCB*)PCB::running)->state=OVER;

		allThreads->cutoff(PCB::running->id);
		((PCB*)PCB::running)->myThread = 0;


		tempstack=((PCB*)PCB::running)->stack;

		runFlag=0;
		lockFlag=0;
	asm popf
	dispatch();


}


PCB::PCB(StackSize stSize, TIMESLICE timeSl) {

	stackSize=stSize;
	timeSlice=timeSl;
	state=MAIN;
	blist=new BlockL();
	myThread=0;
	id=++poslID;
	isSignaled=2;
	numberRequested=0;
	for(int j=0;j<16;j++) locallyBlocked[j]=0;
	for(int i=0;i<16;i++) niz[i]=0;
}
PCB::PCB(Thread *myThr,StackSize stSize,TIMESLICE timeSl){

	if (stSize>MAXSTACK) stackSize=MAXSTACK;
	else stackSize=stSize;
	stackSize = stackSize / sizeof(unsigned);
	stack = new unsigned[stackSize];
	myThread = myThr;
	count=0;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(wrapper);
	stack[stackSize - 3] = FP_OFF(wrapper);
	ss = FP_SEG(stack + stackSize - 12); // stavljanje ss registra na ispod registara opste namene itd
	sp=FP_OFF(stack+stackSize-12);
	bp = FP_OFF(stack+stackSize-12);
	state=CREATED;
	timeSlice = timeSl;
	id=++poslID;
	numberRequested=0;
	asm pushf
	asm cli
	for(int j=0;j<16;j++) locallyBlocked[j]=0;
	for(int i=0;i<MAXSIG;i++) niz[i]=0;
	niz[0]= new FuncElem(&PCB::Handler0);
	blist=new BlockL();
	allThreads->add(myThr,getID());
	if(PCB::running) {
		parent=((PCB*)PCB::running)->myThread;
		for(int i=0;i<MAXSIG;i++) {
			if(running->niz[i]) {
				FuncElem* p=running->niz[i];
				FuncElem* first=niz[i];
				FuncElem* last=0;
				while(p) {
					FuncElem *novi=new FuncElem(p->hand);
					if(first==0) first=novi;
					else last->next=novi;
					last=novi;
					p=p->next;
				}
			}
		}
	}
	asm popf
#endif


}



ID PCB::getID()
{
	return id;
}

Thread * PCB::getThreadwithID(ID id)
{
	return allThreads->ThreadByID(id);
}

ID PCB::runningID()
{
	return running->id;
}


void PCB::wrapper()
{
	running->myThread->run();
	PCB::running->sig[PCB::running->numberRequested++];
	if (running->parent!=0)
		running->parent->myPCB->sig[running->parent->myPCB->numberRequested++];
	asm pushf
	asm cli
	PCB* p=PCB::running->blist->cutoff();



	while(p!=0) {

		p->state=READY;
		Scheduler::put(p);
		p=PCB::running->blist->cutoff();
	}
	PCB::running->state=OVER;
	asm popf
	dispatch();


}
void PCB::waitToComplete() {



	if (state !=OVER) {
		asm pushf
		asm cli
		PCB::running->state=NOTFORSCHEDULER;
		blist->concatenate((PCB*)PCB::running);//mozda potrebno staviti blist->concatenate(static PCB*)

		asm popf
		dispatch();
	}
	//else asm popf
}

PCB::~PCB()
{
	asm pushf
	asm cli

	waitToComplete();

	allThreads->cutoff(this->getID());
	delete stack;
	this->myThread = 0;
	asm popf
}

void PCB::blockSignalGlobally(SignalId signal) {
	asm pushf
	asm cli
	globallyBlocked[signal]=1;
	asm popf
}

void PCB::unblockSignalGlobally(SignalId signal) {
	asm pushf
	asm cli
	globallyBlocked[signal]=0;
	asm popf

}

void PCB::blockSignal(SignalId signal) {
	locallyBlocked[signal]=1;

}

void PCB::unblockSignal(SignalId signal) {


	locallyBlocked[signal]=0;

}


void PCB::signal(SignalId signal) {
	if (signal!=1 && signal!=2){
		sig[numberRequested].number=(int)signal;
		numberRequested++;
	}

}

void PCB::registerHandler(SignalId signal, SignalHandler handler){
	if (signal<MAXSIG){
	FuncElem* elem=new FuncElem(handler);
	if (niz[signal]==0){
		niz[signal]=elem;

	}
	else {

		FuncElem* p=niz[signal];
		while (p->next){
			p=p->next;
		}
		p->next=elem;
	}
	}
}

void PCB::unregisterAllHandlers(SignalId id) {
	FuncElem* p=niz[id];
		while (p){
			FuncElem*	old=p;
			p=p->next;
			delete old;
		}
		niz[id]=0;



}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	int ind1=0, ind2=0;
		FuncElem* p=niz[id], *t1, *t2;
		while (p && (ind1==0 || ind2==0)){
			if (ind1==0 && p->hand==hand1){
				ind1=1;
				t1=p;
			}
			if (ind2==0 && p->hand==hand2){
				ind2=1;
				t2=p;
			}
			p=p->next;
		}
		if (ind1==1 && ind2==1){
			t1->hand=hand2;
			t2->hand=hand1;
		}

}

















