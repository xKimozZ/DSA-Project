#include "Processor.h"
#include"Scheduler.h"

bool Processor::OverHeatedOrNot()
{
	int range1 = 0;     //min. range
	int range2 = 100;   //max. range
	int random = (rand() % (range2 - range1 + 1)) + range1;
	if (random < S->ohprop)
		return true;
	else return false;
}

bool Processor::Status()
{
	return Busy;
}

void Processor::SetStatus(bool s)
{
	Busy = s;
}

void Processor::PrintRUN()
{
	if (running != nullptr)
	{
		cout << running->GetPID();
	}
}

process* Processor::getRUN()
{
	return running;
}

void Processor::SetRUN(process* p)
{
	running = p;
}

void Processor::RemRUN()
{
	SetRUN(NULL);
	SetStatus(false);
}



float Processor::TotLoadCalc()
{
	return 0.0;
}

void Processor::IncRDY()
{
	RDYnum++;
}

void Processor::decRDY()
{
	RDYnum--;
}

float Processor::UtilCalc()
{
	return 0.0;
}

process* Processor::ProcKILL(int id)
{
	return nullptr;
}

process* Processor::runForkKILL(int id)
{
	return nullptr;
}

bool Processor::SignalKillCheck()
{
	return false;
}

int Processor::retCurrentValidSigKill()
{
	return 0;
}

void Processor::DeleteSigKill()
{
	return;
}

void Processor::IncIdle()
{
	IdleTime++;
}

void Processor::IncBusy()
{
	BusyTime++;
}

void Processor::RecalcExp(string op, process* p)
{
	if (op == "R" || op == "r")
		ExpToFinish -= (p->GetCT() - p->GetCRT());
	else if (op == "A" || op == "a")
		ExpToFinish += (p->GetCT() - p->GetCRT());
	else return;
}

float Processor::GetIdle()
{
	return IdleTime;
}

float Processor::GetBusy()
{
	return BusyTime;
}

void Processor::zeroExpT()
{
	ExpToFinish = 0;
}

void Processor::InsertKillListPointer(PriorityQueue<SigKill*>* s, int fp)
{
	return;
}


Processor::Processor(Scheduler *s )
{
	S=s;
	Busy = false;  	//Set idle when first created
	ExpToFinish = 0;
	running = NULL;
}

Processor::Processor()
{
}

Processor::~Processor()
{
}