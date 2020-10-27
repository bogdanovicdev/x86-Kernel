/*
 * ALLTHR.cpp
 *
 *  Created on: Sep 16, 2019
 *      Author: OS1
 */

#include "ALLTHR.h"
#include "THREAD.H"
#include <iostream.h>

ALLTHR::ALLTHR() {
	first=last=0;

}

ALLTHR::~ALLTHR() {
	Elem *temp=first;
	while(temp) {
		Elem *helper=temp;
		temp=temp->next;
		helper->next=0;
		asm pushf
		asm cli
		delete helper;
		asm popf
	}
	first=last=0;
}

PCB* ALLTHR::cutoff(ID id) {

	Elem* temp=first;
	Elem* onebefore=0;
	while(temp) {
		if (temp->myID==id) break;
		onebefore=temp;
		temp=temp->next;

	}
	if (onebefore==0) {
		first=first->next;
		if (first==0) last=0;
	}
	else onebefore->next=temp->next;
	temp->next=0;
	return temp->thread->myPCB;
}
Thread* ALLTHR::ThreadByID(ID id) {
		Elem* temp=first;
		while(temp) {
			if (temp->myID==id) return temp->thread;
		}

		return 0;
}
void ALLTHR::add(Thread *thr, ID id){
	asm pushf
	asm cli
	Elem *novi= new Elem(thr,id);
	asm popf
	if (first==0) {
		first=novi;
	}
	else last->next=novi;
	last=novi;
}


