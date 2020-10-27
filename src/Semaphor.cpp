/*
 * Semaphor.cpp
 *
 *  Created on: Jan 19, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
AllSem* allSemaphores=new AllSem();
Semaphore::Semaphore(int init) {

	myImpl=new KernelS(init,this);
	asm pushf
	asm cli
	allSemaphores->addSemaphor(this);
	asm popf
}

Semaphore::~Semaphore(){
	asm pushf
	asm cli
	allSemaphores->removeSemaphor(this);
	delete myImpl;
	asm popf

}

int Semaphore::val() const {

	return myImpl->value;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);


}

int Semaphore::signal(int n) {

	return myImpl->signal(n);
}

