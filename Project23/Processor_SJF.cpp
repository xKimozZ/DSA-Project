#include "Processor_SJF.h";
#include"Scheduler.h"




float Processor_SJF::TotLoadCalc()
{
	return GetBusy() / S->getTotalTD();
}

float Processor_SJF::UtilCalc()
{
	return GetBusy() / (GetBusy() + GetIdle());
}
void Processor_SJF::ScheduleAlgo()	//Incomplete function: Check above notes
{

	if (!Status()) //processor not busy, no running tasks

	{

		if (RDYTask.getHead()) //if there's a task ready in the queue
		{
			RDYToRUN();
		}
		if (!Status()) IncIdle();
	}

	if (Status())	//If processor is busy (is executing a task)
	{
		
		if (getRUN()->GetCT() > getRUN()->GetCRT()) 	//If the running task is not over yet
			if (getRUN()->GetNum_IO() > 0)
			{//If it's CRT equals IO_R
				if (getRUN()->GetIO()->io_time == getRUN()->GetCRT())
				{
				
					if (RDYEmpty())
						IncIdle();
					else IncBusy();
					process* p = getRUN();
					SetStatus(false);
					getRUN()->SetState("BLK");
					S->IO_handling(this->getRUN());
					SetRUN(nullptr);
					RecalcExp("r", p);
				}
				else
				{
					IncRUN(); //Increment its CRT
					IncBusy();
					ExpToFinish--;
				}
			}
			else
			{
				IncRUN(); //Increment its CRT
				IncBusy();
				ExpToFinish--;
			}



		else if (getRUN()->GetCT() == getRUN()->GetCRT())	//If it's CRT equals CT (supposed to be over)
		{
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			SetStatus(false);							//CPU no longer busy
			S->killOrph(getRUN());			//Kill all children (in FCFS) of a forked process incase it finds its way out of FCFS
			getRUN()->SetState("TRM");					//Set state as TRM
			S->InsertTRM(getRUN());
			SetRUN(nullptr);
			//What I intend from here, is a check in the scheduler.
			//If processor is not busy but has a running process (which should be terminated anyway)
			//Then the scheduler will take it from the processor and deal with it afterwards depending on the set state
		}

	}

}
void Processor_SJF::IncRUN()
{
	getRUN()->incCRT();	//Increment CRT
}
bool Processor_SJF::RDYEmpty()
{
	if (RDYTask.isEmpty())
		return true;
	else
		return false;

}
void Processor_SJF::RDYToRUN()
{
	if (RDYEmpty())
		return;
	SetRUN(RDYTask.dequeue());
	SetStatus(true);
	if (getRUN()->GetCRT() == 0) //If process was never RUN before
		S->UpdateRT(getRUN());	//Calculate its response time
	getRUN()->SetState("RUN");

	RDYnum--;
}
void Processor_SJF::PrintRDY()
{
	RDYTask.print();
}
Processor_SJF::Processor_SJF(Scheduler* s) : Processor(s)
{
}



void Processor_SJF::InsertToRDY(process* p)
{ 
	if (!p) return;
	RDYnum++;
	RDYTask.enqueue(p);
	p->SetState("RDY");
	RecalcExp("a", p);
	RDYTask.sortByCT();
}

void Processor_SJF::resetProcesses()
{
	RDYTask.resetStatus();
}

void Processor_SJF::CalcExpectedTime()
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

process* Processor_SJF::GetProcessFromRdy()
{
	if (RDYEmpty())
		return nullptr;
	RDYnum--;
	RecalcExp("r", RDYTask.getHead()->getItem());
	return RDYTask.dequeue();
}

float Processor_SJF::ExpectedTime()
{
	return ExpToFinish;
}
