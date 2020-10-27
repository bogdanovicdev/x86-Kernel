/*
 * MAIN.CPP
 *
 *  Created on: Jan 20, 2019
 *      Author: OS1
 */

#include "KERNEL.H"
#include <iostream.h>
#include <dos.h>
#include "USER.H"


Idle* idleThread=0;
extern int userMain(int argc,char* argv[]);
int main(int argc, char* argv[]){
	PCB* main_Thread=new PCB(0,defaultStackSize,4);
	PCB::running=main_Thread;

	inic();
	idleThread=new Idle();
	cout<<"Milijan Bogdanovic OPERATIVNI SISTEMI PROJEKAT 220/17\n";
    int val=userMain(argc,argv);
    delete idleThread;
	restore();
	delete main_Thread;
	return val;
}



