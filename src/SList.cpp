/*
 * SList.cpp
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#include "SList.h"

SList::SList(){
	first=last=0;


}
SList::~SList(){
	asm pushf
	asm cli
	Elem *temp=first;
	while (temp) {
		Elem *helper=temp;
		temp=temp->next;
		helper->next=0;
		delete helper;


	}
	first=last=0;
	asm popf
}



void SList::addtoBlocked(PCB* podd,Time timeBl){
	asm pushf
	asm cli
	//stititi?

	Elem *novi=new Elem(podd,timeBl);

	if (first==0) {

		first=novi;

	}else last->next=novi;
	last=novi;
	asm popf


}

void SList::tickList() {

	Elem *temp=first,*onebefore=0;
	while(temp) {
		if (temp->timeBlocked!=0) {
			temp->timeBlocked--;
			if (temp->timeBlocked==0) {
				PCB* zavadjenje=temp->pod;
				if (onebefore==0) {
					first=temp->next;
					temp->next=0;
					temp=first;

				}else {
					onebefore->next=temp->next;
					temp->next=0;
					temp=onebefore->next;

				}
				zavadjenje->state=READY;
				zavadjenje->isSignaled=0;
				Scheduler::put(zavadjenje);


			}else {
				onebefore=temp;
				temp=temp->next;
			}


		}else {

			onebefore=temp;
			temp=temp->next;
		}


	}

}


PCB* SList::cutoff() {

	asm pushf
	asm cli
	if (first!=0){
		PCB* p=first->pod;
		 Elem* pom=first;
		 first=first->next;
		 pom->next=0;
		if (first==0) last=0;
		asm popf
		return p;
	} else {
		asm popf
		return 0;
	}
}


//pod znakom upitnika da li cemo ostaviti ovo
void SList::takeoffBlocked(ID id){
	asm pushf
	asm cli
	Elem *temp=first,*onebefore=0;
	while (temp) {
		if (temp->pod->id==id) break;
		onebefore=temp;
		temp=temp->next;
	}
	if (onebefore==0){

		first=first->next;
		first->next=0;
	}
	else {

		onebefore->next=temp->next;

	}
	asm popf


}










