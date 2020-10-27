/*
 * Semaphor.h
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
#include "KERNELS.H"
#include "ALLSEM.H"
typedef unsigned int Time;


class KernelS;
class AllSem;
class Semaphore {
public:
	Semaphore(int init=1);
	virtual ~Semaphore();


	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);

	int val() const;

	KernelS* myImpl;


};

#endif  SEMAPHOR_H_

