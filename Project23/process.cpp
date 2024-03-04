#include "process.h"
#include "process.h"
#include "process.h"
#include "process.h"
#include<string>
#include<fstream>
#include<iostream>
using namespace std;

process::process(process* par, int at, int id, int ct)
{
	PID = id;
	IO_D = 0;
	AT = at; RT = 0; CT = ct; TT = 0; TD = 0; WT = 0; CRT = 0; NR = 0;
	ChangeOK = true; parent = par; child = nullptr; child = nullptr; forkedOnce = false;
}

process::process()
{
	PID = 0;
	IO_D = 0;
	AT = 0; RT = 0; CT = 0; TT = 0; TD = 0; WT = 0; CRT = 0; NR = 0;
	ChangeOK = true; parent = nullptr; child = nullptr; child1 = nullptr;
}

process::~process()
{

}


void process::SetState(string s)
{
	if (s == "New" || s == "new")
		ST = NEW;
	else if (s == "Rdy" || s == "RDY")
		ST = RDY;
	else if (s == "BLK" || s == "Blk")
		ST = BLK;
	else if (s == "TRM" || s == "Trm")
		ST = TRM;
	else if (s == "ORPH" || s == "Orph")
		ST = ORPH;
	else if (s == "RUN" || s == "run")
		ST = RUN;

}

void process::SetRT(int t)
{
	if (IsBLK == false)
		RT = t - AT;
	else 
		return;
}

int process::GetAT()
{
	return AT;
}

int process::GetPID()
{
	return PID;
}

int process::GetRT()
{
	return RT;
}

void process::Load(ifstream& inf)
{
	IO_REQ* i;
	int it;
	int id;
	int nr=0;
	inf >> AT >> PID >> CT >> DL>>NR ;
	nr = NR;
	while(nr>0)
	{
		i = new IO_REQ;
		inf.ignore(1000, '(');
		inf >> it;
		i->io_time=it;
		inf.ignore(1000, ',');
		inf >> id;
		i->io_dur=id;
		inf.ignore(1000, ')');
		i->ac_dur = id;
		r.enqueue(i);
		r.sortByIO();
		nr--;
     }
}

int process::rec_retDeepestID(process* p)
{
	if (p->child && !(p->child->isTRM()))
		return rec_retDeepestID(p->child);
	else if (p->child1 && !(p->child1->isTRM()))
		return rec_retDeepestID(p->child1);
	else
		return p->PID;
}

void process::Save(ofstream& outf)
{
	outf << TT << " " << PID << " " << CT << " " << AT << " " << IO_D <<" "<< WT << " " << RT << " " << TD << endl;//---->to be continued
}

int process::GetNum_IO()
{
	return NR;
}

IO_REQ* process::GetIO()
{
	IO_REQ* n;
	Node<IO_REQ*>* ptr = r.getHead();
	if (NR != 0)
	{
		n = ptr->getItem();
		return n;
	}
	else
		return nullptr;

}


int process::GetCT()
{
	return CT;
}

void process::ToggleChange()
{
	if (ChangeOK == true)
		ChangeOK = false;
	else
		ChangeOK = true;
}

bool process::isChange()
{
	return ChangeOK;
}

void process::setChild(process* nchild)
{
	if (!this->child && !this->child1)
		this->child = nchild;
	else if (this->child && !this->child1)
		this->child1 = nchild;
	if (this->child && this->child1)
	forkedOnce = true;
}

int process::GetCRT()
{
	return CRT;
}

int process::GetDL()
{
	return DL;
}

int process::GetWT()
{
	return WT;
}

int process::GetTT()
{
	return TT;
}

int process::GetTD()
{
	return TD;
}

void process::RemoveIO()
{
	IO_D += r.getHead()->getItem()->ac_dur;
	delete r.dequeue();
	NR--;
}

int process::returnIO_D()
{
	return IO_D;
}

void process::SetTT(int t)
{
	TT = t;
	TD = TT - AT;
	WT = TD - CRT;
	if (WT < 0)
		WT = 0;
}

void process::incCRT()
{
	CRT++;
}

bool process::isTRM()
{
	return (ST == TRM);
}

bool process::isRUN()
{
	return (ST == RUN);
}

int process::retDeepestID()
{
	return rec_retDeepestID(this);
}

bool process::OrphStillExists()
{
	if (child && !child->isTRM() || child1 && !child1->isTRM())
		return true;
	else return false;
}

bool process::ProcessForkAttempt(int ForkProb)
{
	if (forkedOnce) return false;
	//Insert randomization sequence here
	int range1 = 0;     //min. range
	int range2 = 100;   //max. range
	int random = (rand() % (range2 - range1 + 1)) + range1;
	if (random < ForkProb)
		return true;
	else return false;
}

bool process::IsForked()
{
	return forkedOnce;

}

bool process::IsChild()
{
	if (parent)
		return true;
	else return false;

}