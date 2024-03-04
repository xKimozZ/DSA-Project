#include "Processor_FCFS.h";
#include"Scheduler.h"



void Processor_FCFS::CalcExpectedTime()
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

float Processor_FCFS::ExpectedTime()
{
	return ExpToFinish;
}

void Processor_FCFS::InsertToRDY(process* p)
{
	if (!p) return;
	RDYnum++;
	RDYTask.insert(p);
	p->SetState("RDY");
	RecalcExp("a", p);

}

float Processor_FCFS::TotLoadCalc()
{
	return GetBusy() / S->getTotalTD();
}

float Processor_FCFS::UtilCalc()
{
	return GetBusy() / (GetBusy() + GetIdle());
}

void Processor_FCFS::IncRUN()
{
	getRUN()->incCRT();	//Increment CRT
}

int Processor_FCFS::getSigKillID()
{
	if (KillList->getHead())
		if (KillList->getHead()->getItem())
			return KillList->getHead()->getItem()->ID;
	return -1;
}

bool Processor_FCFS::SignalKillCheck()
{
	if (!KillList->getHead()) return false;						//Safety check
	SigKill* s = KillList->getHead()->getItem();
	if (S->SigKillGreenlight(s))		//Is the first SIGKILL request's scheduled time here yet?
	{
		int tmpID = getSigKillID();
		if (RDYTask.getByID(tmpID)) {					//If the ID is found
			SigKilled = ProcKILL(tmpID);				//Set SigKilled to the found process (will be dealt with by scheduler)
			S->killOrph(SigKilled);
			S->InsertTRM(SigKilled);
			return true;
		}
		else if (getRUN())								//Checks if RUN process exists
			if (getRUN()->GetPID() == tmpID)				//Checks if IDs match
			{
				SigKilled = runForkKILL(tmpID);				//Set SigKilled to the found process (will be dealt with by scheduler)
				S->killOrph(SigKilled);
				S->InsertTRM(SigKilled);
				return true;
			}
			else return false; 
		else return false;
	}
	else return false;
}

void Processor_FCFS::DeleteSigKill()
{
	if (KillList->getHead())
	delete KillList->dequeue();
}

int Processor_FCFS::retCurrentValidSigKill()
{
	if (!KillList->getHead())
		return 0;
	Node<SigKill*>* currNode = KillList->getHead();
	SigKill* s = currNode->getItem();
	int i=0;
	while (currNode && S->SigKillGreenlight(s))
	{
		i++;
		currNode = currNode->getNext();
		if (currNode)
		s = currNode->getItem();
	}
	return i;
}

process* Processor_FCFS::retSigKilled()
{
	if (!SigKilled)
		return nullptr;
	return SigKilled;
}

void Processor_FCFS::resetSK()
{
	SigKilled = nullptr;
}

//Note: GetCRT() is missing in process & SetTT is supposed to take an int parameter
//IO_Queue doesn't exist yet. I suggest it be a queue of IO_R objects in process class

void Processor_FCFS::ScheduleAlgo()	//Incomplete function: Check above notes
{
	/*if (OverHeatedOrNot())
	{
		stop = true;
		
		
		if (!RDYTask.isEmpty()||!getRUN()) {
			Node<process*>* ptr = RDYTask.getHead();
			if (S->GetNUMProcessors() != 1) {
				S->sendtoShortest(getRUN());
				RemRUN();
				while (!RDYTask.isEmpty()) {
					S->sendtoShortest(ptr->getItem());
					ptr = ptr->getNext();
				}
			}
			else if (S->GetNUMProcessors() != 1) {
				S->sendtoNEW(getRUN());
				RemRUN();
				while (!RDYTask.isEmpty()) {
					S->sendtoNEW(ptr->getItem());
					ptr = ptr->getNext();
				}
			}
		}
		if (ORT != S->oht)
			ORT++;
		else
			stop == false;
	}
	else*/

		if (!Status())//processor not busy, no running tasks

		{

			if (RDYTask.getHead()) //if there's a task ready in the queue
			{
				if (RDYTask.getHead() && (S->FindShortestRR() != -1)) //If RDY queue is not empty, and RR processors exist
				{
					int TWT = S->CalcTWT(RDYTask.getHead()->getItem());
					while (TWT > S->GetMaxW() && RDYTask.getHead())
					{

						process* prc = RDYTask.remove();
						RDYnum--;
						RecalcExp("r", prc);
						if (prc->IsChild())	//If a non FCFS processor will receive child, reverse the operation and run the process instead
						{
							SetRUN(prc);
							SetStatus(true);
							if (getRUN()->GetCRT() == 0) //If process was never RUN before
								S->UpdateRT(getRUN());	//Calculate its response time
							getRUN()->SetState("RUN");
							RecalcExp("a", prc);			//Reverse change to ExpToFinish
							//getRUN()->ToggleChange();      <--------State transition constraint lifted!
							break;
						}
						else
						{
							S->migrationprocess_from_FCFS_To_RR(prc);
							if (RDYTask.getHead())
							{
								TWT = S->CalcTWT(RDYTask.getHead()->getItem());
							}
							else
								this->zeroExpT();
						}
					}
					if (RDYTask.getHead() && !Status())
						RDYToRUN();
				}
				else
					RDYToRUN();
			}
			if (!Status())
				IncIdle();
		}


		if (getRUN() && Status())		//Check if processor is running something
		{
			if (getRUN()->ProcessForkAttempt(*ForkProb))	//Checks if process will try to fork
			{
				S->ForkProcess(getRUN());				//Inform scheduler to fork the process
			} //Scheduler work
		}

		if (Status())	//If processor is busy (is executing a tasl)
		{
			if (getRUN()->GetCT() > getRUN()->GetCRT())	//If the running task is not over yet
				if (getRUN()->GetNum_IO() > 0)
				{//If it's CRT equals IO_R
					if (getRUN()->GetIO()->io_time == getRUN()->GetCRT())
					{
						if (RDYEmpty())
							IncIdle();									//Increment idle time as not actively executing process
						else IncBusy();
						process* p = getRUN();
						SetStatus(false);
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

				}
				else
				{
					IncRUN();								//Increment its CRT
					IncBusy();
					ExpToFinish--;
				}
			else if (getRUN()->GetCT() == getRUN()->GetCRT())	//If it's CRT equals CT (supposed to be over)
			{
				if (RDYEmpty())
					IncIdle();
				else IncBusy();
				SetStatus(false);							//CPU no longer busy
				S->killOrph(getRUN());
				S->InsertTRM(getRUN());					//Set state as TRM, move into TRM list
				SetRUN(nullptr);
				//What I intend from here, is a check in the scheduler.
				//If processor is not busy but has a running process (which should be terminated anyway)
				//Then the scheduler will take it from the processor and deal with it afterwards depending on the set state
			}

		}
}

bool Processor_FCFS::RDYEmpty()
{
	if (RDYTask.isEmpty())
		return true;
	else
		return false;
}

void Processor_FCFS::RDYToRUN()
{
	if (RDYEmpty())
		return;
	RDYnum--;
	
	SetRUN(RDYTask.remove());
	SetStatus(true);
	if (getRUN()->GetCRT() == 0) //If process was never RUN before
		S->UpdateRT(getRUN());	//Calculate its response time
	getRUN()->SetState("RUN");
}

void Processor_FCFS::PrintRDY()
{
	RDYTask.print();
}


process* Processor_FCFS::ProcKILL(int id)
{
	Node<process*>* tmpNode = RDYTask.getByID(id);
	if (tmpNode)
	{
			RDYnum--;
			S->IncKilled();
			RecalcExp("r", tmpNode->getItem());
	}
	else
		return nullptr;
	return RDYTask.findAndRemove(id);

}

process* Processor_FCFS::runForkKILL(int id)
{
	if (getRUN())
		if (getRUN()->GetPID() == id)
		{
			process* p = getRUN();
			RecalcExp("r", p);
			SetStatus(false);							//CPU no longer busy
			SetRUN(nullptr);						//Set running as nullptr
			S->IncKilled();
			return p;
		}
	return nullptr;
}

void Processor_FCFS::resetProcesses()
{
	RDYTask.resetStatus();
}

Processor_FCFS::Processor_FCFS(Scheduler* S): Processor(S)
{
	SigKilled = nullptr;
	ForkProb = new int;
}

process* Processor_FCFS::GetProcessFromRdy()
{
	Node<process*>* prc;
	prc = RDYTask.getHead();
	while (prc)
	{
			int id = prc->getItem()->GetPID();
			process* stolen = ProcKILL(id);
			return stolen;
	}
	return nullptr;

}

void Processor_FCFS::InsertKillListPointer(PriorityQueue<SigKill*>* s, int fp)
{
	KillList = s;
	*ForkProb = fp;
}