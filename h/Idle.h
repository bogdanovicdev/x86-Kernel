/*
 * Idle.h
 *
 *  Created on: Jan 2, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.H"

class Idle:public Thread{
public:
	Idle();
	void run();
	~Idle();
};

#endif /* IDLE_H_ */
