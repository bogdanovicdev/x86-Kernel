/*
 * KernelS.cpp
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#include "KernelS.h"
#include "SEMAPHOR.H"
void dispatch();


KernelS::KernelS(int init,Semaphore* myS){
	//cout<<"napravljen semafor\n";
	mySem=myS;
	value=init;
	//zastititi mozda?
	slist=new SList();



}

 KernelS::~KernelS() {
		delete slist;
		mySem=0;


}

int KernelS::wait(Time maxTimeToWait){
	asm pushf
	asm cli
	if (--value<0) {
		PCB::running->state=NOTFORSCHEDULER;//moze i NOTFORSCHEDULER
		slist->addtoBlocked((PCB*)PCB::running,maxTimeToWait);//! promeni metodu
		asm popf
		dispatch();
		if(!PCB::running->isSignaled) {return 0;}
		else{return 1;}

	}else{


	asm popf
	return 1;
	}

}




int KernelS::signal(int n=0) {

	int pomocnisem=n;
	asm pushf
	asm cli
	if (n==0) {
		if (++value<=0){
		PCB* pom=slist->cutoff();
		pom->state=READY;
		pom->isSignaled=1;
		Scheduler::put(pom);
	}
		asm popf;
		return 0;
	}
	if (n<0) {
		asm popf;
		return n;
	}


	if (value>=0){
		value=value+n;
		asm popf;
		return 0;
	}

	 if (-value<n) pomocnisem=-value;
	 value=value+n;
	 int pom=pomocnisem;

	 PCB* p;
	 while(pomocnisem>0){
		 p=slist->cutoff();
		 p->state=READY;
		 p->isSignaled=1;
		 Scheduler::put(p);

		 pomocnisem--;
	 }
	 asm popf;
	 return pom;

}
void KernelS::tickSemaphor() {
	//asm pushf
	//asm cli
	// da li treba okruziti?
	slist->tickList();

	//asm popf
}

int KernelS::val() const {

	return value;

}


