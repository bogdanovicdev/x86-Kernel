/*
 * Event.cpp
 *
 *  Created on: Jan 22, 2020
 *      Author: OS1
 */

#include "Event.h"
#include "KERNEL.H"
#include <dos.h>

IVTEntry* IVTEntry::IVT[KAPACITET];

Event::Event(IVTNo ivtno) {

	myImpl=new KernelEv(ivtno);

}

Event::~Event() {
	delete myImpl;
}

void Event::wait() {

	myImpl->wait();


}

void Event::signal() {

	myImpl->signal();

}

IVTEntry::IVTEntry(IVTNo ivtno,InterruptPointer pointer) {
	asm pushf
	asm cli
	ivtNo=ivtno;
	IVT[ivtno]=this;
	newPt=pointer;
	event=0;

	asm popf


}

void IVTEntry::signal() {
	event->signal();
}

KernelEv::KernelEv(IVTNo number) {
	asm pushf
	asm cli

	whichThread=(PCB*)PCB::running;
	ivtno=number;
	value=0;
	IVTEntry::IVT[ivtno]->event=this;

	setUpNew();


	asm popf;





}
void KernelEv::SewOldBack() {

	setvect(ivtno,(IVTEntry::IVT[ivtno])->oldPt);

}

void KernelEv::SewNew() {


	setvect(ivtno,(IVTEntry::IVT[ivtno])->newPt);

}


void KernelEv::setUpNew() {
	asm pushf
	asm cli
	(IVTEntry::IVT[ivtno])->oldPt=getvect(ivtno);
	SewNew();
	asm popf
}


void KernelEv::restoreOld() {
	asm pushf
	asm cli
	SewOldBack();

	asm popf



}

KernelEv::~KernelEv() {
	restoreOld();

}

void KernelEv::signal() {
	asm pushf
	asm cli

	if(value++<0) {
		whichThread->state=READY;
		Scheduler::put(whichThread);
	}
	asm popf
}

void KernelEv::wait() {
	asm pushf
	asm cli


	if (PCB::running==whichThread) {
		if(--value<0) {
			PCB::running->state=NOTFORSCHEDULER;
			dispatch();
		}
	}
	asm popf

}













