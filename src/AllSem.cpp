/*
 * AllSem.cpp
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#include "AllSem.h"

AllSem::AllSem(){
	first=last=0;


}


volatile void AllSem::addSemaphor(Semaphore* s){
	asm pushf
	asm cli
	Elem* novi=new Elem(s);
	if (first==0) {

		first=last=novi;

	}else last->next=novi;
	last=novi;
	asm popf
}


volatile void AllSem::removeSemaphor(Semaphore* s) {
	asm pushf
	asm cli
	Elem *temp=first,*onebefore=0;
		while (temp) {
			if (temp->sem==s) break;
			onebefore=temp;
			temp=temp->next;
		}
		if (onebefore==0){

			first=first->next;
			
		}
		else {

			onebefore->next=temp->next;

		}
		temp->sem=0;
		temp->next=0;
		delete temp;
		asm popf

}

AllSem::~AllSem(){
	Elem *temp=first;
	while(temp){
		Elem *helper=temp;
		temp=temp->next;
		helper->next=0;
		asm pushf
		asm cli
		delete helper;
		helper=0;
		asm popf
	}
	first=last=0;

}

volatile void AllSem::tickAll() {
	asm pushf
	asm cli
	Elem *temp=first;
	while(temp) {

		temp->sem->myImpl->tickSemaphor();
		temp=temp->next;
	}

	asm popf

}





