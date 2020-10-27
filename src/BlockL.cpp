
#include "BLOCKL.H"
#include <iostream.h>
BlockL::BlockL(){
	first=last=0;
}
void BlockL::concatenate(PCB* p){
	asm pushf
	asm cli
	Elem* novi = new Elem(p);
	asm popf
	if (first==0){
		first=novi;
	}
	else last->next=novi;
	last=novi;
}
PCB* BlockL::cutoff(){
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
BlockL::~BlockL(){
	asm pushf
	asm cli
	while (first){
		Elem* temp=first;
		first=first->next;
		temp->next=0;

		delete temp;

	}
	asm popf
}
