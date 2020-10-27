/*
 * Idle.cpp
 *
 *  Created on: Jan 2, 2020
 *      Author: OS1
 */

#include "Idle.h"
#include "PCB.H"
#include "THREAD.H"
#include "SETUP.H"
#include "SCHEDULE.H"

Idle::Idle():Thread(){};
void Idle::run() {
	while(1) {
		dispatch();

	}
}

Idle::~Idle() {
	myPCB->state=OVER;
};
