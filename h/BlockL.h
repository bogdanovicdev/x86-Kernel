#ifndef BLOCKL_H_
#define BLOCKL_H_
#include "PCB.H"
class PCB;
class BlockL {
private:

	struct Elem{
		PCB* pod;
		Elem* next;
		Elem(PCB* p){
			pod=p;
			next=0;
		}
	};

public:
	Elem* first,*last;
     BlockL();
     void concatenate(PCB* p);
     PCB* cutoff();
     ~BlockL();
};

#endif /* BLOCKL_H_ */
