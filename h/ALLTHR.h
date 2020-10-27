/*
 * ALLTHR.h
 *
 *  Created on: Sep 16, 2019
 *      Author: OS1
 */

#ifndef ALLTHR_H_
#define ALLTHR_H_
#include "THREAD.H"
#include "PCB.H"

class ALLTHR {
	friend class Thread;
	friend class PCB;
private:
	struct Elem {
		Thread *thread;
		ID myID;
		Elem *next;

	public:
		Elem(Thread* thr,ID id) {
			thread=thr;
			myID=id;
			next=0;
		}

	};

public:
	Elem *first, *last;
	ALLTHR();
	virtual ~ALLTHR();
	void add(Thread* thr,ID id);
	PCB* cutoff(ID id);
	Thread* ALLTHR::ThreadByID(ID id);
};


#endif /* ALLTHR_H_ */


