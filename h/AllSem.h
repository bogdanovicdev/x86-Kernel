/*
 * AllSem.h
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#ifndef ALLSEM_H_
#define ALLSEM_H_
#include "KERNELS.H"
#include "SLIST.H"

class Semaphore;



class AllSem {


private:
	struct Elem{
		Semaphore* sem;
		Elem* next;

		public:
		Elem(Semaphore* s) {
			sem=s;
			next=0;
		}

	};




public:
	Elem *first,*last;
	AllSem();
	~AllSem();
	volatile void addSemaphor(Semaphore* s);
	volatile void removeSemaphor(Semaphore* s);
	volatile void tickAll();

};

#endif  ALLSEM_H_
