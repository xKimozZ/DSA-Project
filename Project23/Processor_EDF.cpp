#include "Processor_EDF.h"
#include"Scheduler.h"


float Processor_EDF::TotLoadCalc()
{
	return GetBusy() / S->getTotalTD();
}

float Processor_EDF::UtilCalc()
{
	return GetBusy() / (GetBusy() + GetIdle());
}
void Processor_EDF::ScheduleAlgo()	//Incomplete function: Check above notes
{

	if (!Status()) //processor not busy, no running tasks

	{

		if (RDYTask.getHead()) //if there's a task ready in the queue
		{
			RDYToRUN();
		}
		if (!Status())
		IncIdle();

	}

	if (Status())	//If processor is busy (is executing a task)
	{

		if (getRUN()->GetCT() > getRUN()->GetCRT()) {	//If the running task is not over yet
			IncRUN(); //Increment its CRT
			RDYTask.sortByDL();
			IncBusy();
			ExpToFinish--;
		}

		else if (getRUN()->GetCT() == getRUN()->GetCRT())	//If it's CRT equals CT (supposed to be over)
		{
			SetStatus(false);							//CPU no longer busy
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			S->killOrph(getRUN());			//Kill all children (in FCFS) of a forked process incase it finds its way out of FCFS
			getRUN()->SetState("TRM");					//Set state as TRM
			S->InsertTRM(getRUN());
			SetRUN(nullptr);
			//What I intend from here, is a check in the scheduler.
			//If processor is not busy but has a running process (which should be terminated anyway)
			//Then the scheduler will take it from the processor and deal with it afterwards depending on the set state
		}

		else if (getRUN()->GetIO()->io_time == getRUN()->GetCRT())	//If it's CRT equals IO_R
		{
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			SetStatus(false);
			process* p = getRUN();
			getRUN()->SetState("BLK");					//Set state as BLK
			S->IO_handling(this->getRUN());				//Dequeue a IO_R object (an io request has been carried out)
			SetRUN(nullptr);
			RecalcExp("r", p);
		}

	}

}
void Processor_EDF::IncRUN()
{
	getRUN()->incCRT();	//Increment CRT
}
bool Processor_EDF::RDYEmpty()
{
	if (RDYTask.isEmpty())
		return true;
	else
		return false;

}
void Processor_EDF::RDYToRUN()
{
	if (RDYEmpty())
		return;
	
	{

	  SetRUN(RDYTask.dequeue());
	  SetStatus(true);
	  getRUN()->SetState("RUN");
	  S->UpdateRT(getRUN());
	  RDYnum--;
    }
}
void Processor_EDF::PrintRDY()
{
	RDYTask.print();
}
Processor_EDF::Processor_EDF(Scheduler* s) : Processor(s)
{
}



void Processor_EDF::InsertToRDY(process* p)
{
	if (!p) return;
	RDYnum++;
	RDYTask.enqueue(p);
	p->SetState("RDY");
	RecalcExp("a", p);

}

void Processor_EDF::resetProcesses()
{
	RDYTask.resetStatus();
}

void Processor_EDF::CalcExpectedTime()
{
	Node<process*>* ptr;
	ptr = RDYTask.getHead();
	ExpToFinish = 0;
	while (ptr)
	{
		ExpToFinish += (ptr->getItem()->GetCT() - ptr->getItem()->GetCRT());
		ptr = ptr->getNext();

	}
}

float Processor_EDF::ExpectedTime()
{
	return ExpToFinish;
}
process* Processor_EDF::GetProcessFromRdy()
{
	if (RDYEmpty())
		return nullptr;
	RDYnum--;
	RecalcExp("r", RDYTask.getHead()->getItem());
	return RDYTask.dequeue();
}

