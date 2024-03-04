#include "Processor_RR.h"
#include"Scheduler.h"

void Processor_RR::InsertToRDY(process* p)
{
	if (!p) return;
    RDYnum++;
    RDYTask.enqueue(p);
	RecalcExp("a", p);
    p->SetState("RDY");
}

float Processor_RR::TotLoadCalc()
{
    return GetBusy() / S->getTotalTD();
}

float Processor_RR::UtilCalc()
{
    return GetBusy() / (GetBusy() + GetIdle() ) ;
}

void Processor_RR::ScheduleAlgo()
{

	if (!Status()) //processor not busy, no running tasks

	{

		if (RDYTask.getHead()) //if there's a task ready in the queue
		{
			if (S->FindShortestSJF() != -1)
			{
				int cRTF = RDYTask.getHead()->getItem()->GetCT() - RDYTask.getHead()->getItem()->GetCRT(); //current RTF
				while (cRTF < S->GetRTF() && RDYTask.getHead())
				{
					//if (!RDYTask.getHead()->getItem()->isChange())    <--------State transition constraint lifted!
					//	break;
					RDYnum--;
					RecalcExp("r", RDYTask.getHead()->getItem());
					S->MigrationFromRRToSJF(RDYTask.dequeue());
					if (!RDYTask.getHead())
						this->zeroExpT();
					if (RDYTask.getHead())
						cRTF = RDYTask.getHead()->getItem()->GetCT() - RDYTask.getHead()->getItem()->GetCRT(); //current RTF
				}
			}
			RDYToRUN();
		}
		if (!Status())
		IncIdle();
	}

	if (Status())	//If processor is busy (is executing a tasl)
	{

		if (currentRslice == 0)    //If it's CRT equals CT (supposed to be over)
		{
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			SetStatus(false);
			InsertToRDY(getRUN());                //Insert to ready
			SetRUN(nullptr);
			currentRslice = RT;
		}

		else if (getRUN()->GetCT() > getRUN()->GetCRT())	//If the running task is not over yet
			if (getRUN()->GetNum_IO() > 0)
			{//If it's CRT equals IO_R
				if (getRUN()->GetIO()->io_time == getRUN()->GetCRT())
				{
					if (RDYEmpty())
						IncIdle();
					else IncBusy();
					SetStatus(false);
					process* p = getRUN();
					getRUN()->SetState("BLK");					//Set state as BLK
					S->IO_handling(this->getRUN());
					SetRUN(nullptr);	
					RecalcExp("r", p);//Dequeue a IO_R object (an io request has been carried out)
				}
				else
				{
					IncRUN();
					IncBusy();
					ExpToFinish--;
				}
				currentRslice--;

			}
			else
			{
				IncRUN();
				IncBusy();								//Increment its CRT
				currentRslice--;
				ExpToFinish--;
			}
		else if (getRUN()->GetCT() == getRUN()->GetCRT())	//If it's CRT equals CT (supposed to be over)
		{
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			SetStatus(false);							//CPU no longer busy
			S->killOrph(getRUN());			//Kill all children (in FCFS) of a forked process incase it finds its way out of FCFS
			getRUN()->SetState("TRM");	     			//Set state as TRM
			S->InsertTRM(getRUN());					//Set state as TRM, move into TRM list
			SetRUN(nullptr);
			//What I intend from here, is a check in the scheduler.
			//If processor is not busy but has a running process (which should be terminated anyway)
			//Then the scheduler will take it from the processor and deal with it afterwards depending on the set state
		}

		else if (getRUN()->GetIO()->io_time == getRUN()->GetCRT())	//If it's CRT equals IO_R
		{
			SetStatus(false);
			if (RDYEmpty())
				IncIdle();
			else IncBusy();
			getRUN()->SetState("BLK");					//Set state as BLK
			S->IO_handling(this->getRUN());				//Dequeue a IO_R object (an io request has been carried out)
		}

	}
	
}

bool Processor_RR::RDYEmpty()
{
    if (RDYTask.isEmpty())
        return true;
    else
        return false;
}

void Processor_RR::RDYToRUN()
{
	if (RDYEmpty())
		return;
	
	RDYnum--;
	
    SetRUN(RDYTask.dequeue());
    SetStatus(true);
	if (getRUN()->GetCRT() == 0) //If process was never RUN before
		S->UpdateRT(getRUN());	//Calculate its response time
	getRUN()->SetState("RUN");
	
}

void Processor_RR::PrintRDY()
{
    RDYTask.print();
}

void Processor_RR::resetProcesses()
{
	RDYTask.resetStatus();
}

void Processor_RR::CalcExpectedTime()
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

float Processor_RR::ExpectedTime()
{
	return ExpToFinish;
}

void Processor_RR::IncRUN()
{
	getRUN()->incCRT();	//Increment CRT
}

Processor_RR::Processor_RR(Scheduler* s, int rt) : Processor(s)
{
	RT = rt;
	currentRslice = rt;
}

process* Processor_RR::GetProcessFromRdy()
{
	if (RDYEmpty())
		return nullptr;
	RDYnum--;
	RecalcExp("r", RDYTask.getHead()->getItem());
	return RDYTask.dequeue();
}
