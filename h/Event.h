/*
 * Event.h
 *
 *  Created on: Jan 22, 2020
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "KERNEL.H"
const int KAPACITET=256;
typedef unsigned char IVTNo;
typedef void interrupt(*InterruptPointer)(...);
class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};



class KernelEv {
public:
	int value;
	IVTNo ivtno;
	PCB* whichThread;
	void setUpNew();
	void restoreOld();
	void SewOldBack();
	void SewNew();
	void signal();
	void wait();

	KernelEv(IVTNo number);
	~KernelEv();

};

class IVTEntry {

public:
	static IVTEntry* IVT[];
	InterruptPointer oldPt;
	InterruptPointer newPt;
	KernelEv* event;
	IVTNo ivtNo;


	IVTEntry(IVTNo ivtNo,InterruptPointer pointer);
	void signal();
};
#define PREPAREENTRY(numEntry,callOld)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
		newEntry##numEntry.signal();\
		if(callOld==1)\
		newEntry##numEntry.oldPt();\
	}
















#endif /* EVENT_H_ */
