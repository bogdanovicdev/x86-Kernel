/*
 * SList.h
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#ifndef SLIST_H_
#define SLIST_H_
#include "PCB.H"
#include "KERNELS.H"
#include "SEMAPHOR.H"
typedef unsigned int Time;

class SList {

private:
	struct Elem {
		PCB* pod;
		Elem* next;
		Time timeBlocked;

	public:
		Elem(PCB* podd,Time timeBl) {
			pod=podd;
			timeBlocked=timeBl;

			next=0;
		}

	};
public:
	Elem* first,*last;
	void addtoBlocked(PCB* podd,Time timeBl);
	void takeoffBlocked(ID id);
	PCB* cutoff();
	SList();
	~SList();
	void tickList();

};

#endif /* SLIST_H_ */
