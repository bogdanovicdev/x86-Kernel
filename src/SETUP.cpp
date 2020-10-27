/*
 * SETUP.cpp
 *
 *  Created on: Sep 16, 2019
 *      Author: OS1
 */
#include "SETUP.H"
#include "SCHEDULE.H"
#include "IDLE.H"
#include <iostream.h>

#include "PCB.H"
#include "KERNELS.H"
#include "ALLSEM.H"

class AllSem;
class Idle;
unsigned int tbp,tsp,tss;
extern Idle* idleThread;
extern Semaphore s1;
extern volatile AllSem* allSemaphores;
volatile int lockFlag=0;
volatile int runFlag=0;
volatile PCB* hrunning=0;
extern void TestSignal();
unsigned pomocni[MAXSIG];
volatile unsigned *tempstack=0;
int broj,ind,k;
unsigned int oldTimerSEG, oldTimerOFF;
extern void tick();
extern volatile PCB* monitor;
extern volatile int context_on_demand;
volatile int clock=5;

void inic(){

	asm{
				cli
				push es
				push ax

				mov ax,0   //  ; inicijalizuje rutinu za tajmer
				mov es,ax

				mov ax, word ptr es : 0022h //; pamti staru rutinu
				mov word ptr oldTimerSEG, ax
				mov ax, word ptr es : 0020h
				mov word ptr oldTimerOFF, ax

				mov word ptr es : 0022h, seg timer	 //postavlja
				mov word ptr es : 0020h, offset timer //novu rutinu

				mov ax, oldTimerSEG	 //	postavlja staru rutinu
				mov word ptr es : 0182h, ax //; na int 60h
				mov ax, oldTimerOFF
				mov word ptr es : 0180h, ax

				pop ax
				pop es
				sti

	}
}

void interrupt timer(...){	// OVDE NAPRAVI IZMENE

	tick();
	if (!context_on_demand){
		if(runFlag==0){
			clock--;
		}
	allSemaphores->tickAll();
	}
	if ((clock == 0)  || context_on_demand) {
		if(lockFlag==0){
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp,bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp=tbp;


		if ((PCB::running->state!=OVER) && (PCB::running->state!=NOTFORSCHEDULER)) {

			Scheduler::put((PCB*)PCB::running);

		}
		PCB::running=(PCB*)Scheduler::get();
		if(PCB::running==0) {
			PCB::running=(volatile PCB*)idleThread->myPCB;
			PCB::running->state=NOTFORSCHEDULER;
		}

		tsp=PCB::running->sp;
		tss=PCB::running->ss;
		tbp=PCB::running->bp;
		clock=PCB::running->timeSlice;
			asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp,tbp
			}
			/*if(tempstack!=0){
				delete (unsigned*) tempstack;
				tempstack=0;


			}*/

		}
	}

	if (context_on_demand) {
				context_on_demand=0;
			}else asm int 60h


	if(runFlag==0) {
		runFlag=1;
		lockFlag=1;

		for(int i=0;i<MAXNUMSIG;i++) {
			if(PCB::running->sig[i].number!=-1 &&
				PCB::running->sig[i].completed==0 &&
				(PCB::running->locallyBlocked[PCB::running->sig[i].number]!=1) &&
				PCB::globallyBlocked[PCB::running->sig[i].number]!=1) {
				while(PCB::running->niz[PCB::running->sig[i].number]) {
					asm sti;

					PCB::running->niz[PCB::running->sig[i].number]->hand();

					PCB::running->niz[PCB::running->sig[i].number]=
					PCB::running->niz[PCB::running->sig[i].number]->next;


				}

				PCB::running->sig[i].completed=1;

			}


		}



		lockFlag=0;
		runFlag=0;
	}
}


void restore() {
			asm{
				cli
				push es
				push ax

				mov ax,0
				mov es,ax


				mov ax, word ptr oldTimerSEG
				mov word ptr es : 0022h, ax
				mov ax, word ptr oldTimerOFF
				mov word ptr es : 0020h, ax

				pop ax
				pop es
				sti
			}
	}




