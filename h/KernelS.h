/*
 * KernelS.h
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#ifndef KERNELS_H_
#define KERNELS_H_
#include "PCB.H"
#include "SETUP.H"
#include "SEMAPHOR.H"
#include "SLIST.H"
#include "SCHEDULE.H"
#include "ALLSEM.H"

class AllSem;
class Semaphor;
class SList;




class KernelS {
public:
	int value;
	KernelS(int init=1,Semaphore *myS);
	virtual ~KernelS();

	int wait(Time maxTimeToWait);
	int signal(int n);
	void tickSemaphor();
	int val() const;
	SList* slist;
	Semaphore* mySem;


};

#endif KERNELS_H_
