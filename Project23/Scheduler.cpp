#include "Scheduler.h"
#include "UI.h"

void Scheduler::sendtoShortest(process* p)
{
	int x = FindShortestQueue();
	Processors[x]->InsertToRDY(p);
}

void Scheduler::sendtoNEW(process* p)
{
	NEW.enqueue(p);
}

void Scheduler::killOrph(process* p)
{
	while (p->OrphStillExists())	//While orphan processes still exist
	{
		process* orph = nullptr;
		int id = p->retDeepestID();	//Returns deepest child's ID
		for (int i = 0; i < numProcessor; i++)	//Loops all processors
		{
			if ((orph = Processors[i]->ProcKILL(id)) || (orph = Processors[i]->runForkKILL(id)))	//ProcKILL is a virtual function. FCFS exclusive. Returns the process of the ID if found
				//And so is runForkKILL. Which checks the running process.
			{
				orph->SetTT(TS);					//Sets termination time as current timestep
				InsertTRM(orph);					//Inserts into TRM list, and updates its state as TRM
				break;
			}
		}
	}
}

void Scheduler::simulate(string file)
{

	UI* temp = new UI;
	LoadAction(file);      //starts by calling load function to load from input file
	NEWnum = 0;
	{									//Error detection sequence
		temp->Error( ErrorCheck() );
		if (ErrorCheck() != 0)
		return;
	}
	temp->Type();


	while (numProc !=TRMnum   ) {
		                
		NEWToRDY();
		if (temp->t != 3)
		temp->show(this,TS);
		 BLKToRDY();
		for (int i = 0; i < numProcessor; i++) //loop to start inserting process to Run of each processor 
		{
				Processors[i]->ScheduleAlgo();	
		}

		int ValidSK = 0;						//Temporary value that stores the number of valid concurrent sigkills at current timestep
		for (int i = 0; i < numProcessor; i++)
		{
			if (dynamic_cast<Processor_FCFS*>(Processors[i]))
			{
				ValidSK = Processors[i]->retCurrentValidSigKill();		//Returns the number of valid sigkills at the timestep
				break;													//Breaks in order not to overwrite the value
			}
		}

		while (ValidSK > 0)												//While valid SigKills still exist
		{
			for (int i = 0; i < numProcessor; i++) //Call SigKILL in all FCFS processors (does nothing otherwise)
			{
				if ( Processors[i]->SignalKillCheck() )					//Breaks when one SigKill request is correctly processed
					break;
			}
			ValidSK--;													//Decrement the number
			for (int i = 0; i < numProcessor; i++)
			{
				if (dynamic_cast<Processor_FCFS*>(Processors[i]))
				{
					Processors[i]->DeleteSigKill();						//Deletes JUST one SigKill after the request is processed, whether successful or failed
					break;
				}
			}
		}

		if (STL > 0)
		if (TS % STL == 0)
		{
			WorkStealing();
		}
		RUNnum = CalcRUNnum();
		if (temp-> t != 3)
		   temp->show(this,TS);
		TS++;
	}
	if (temp->t == 3)
		temp->show(this,TS);
	ofstream outFile("LFOUT6.txt");
	SaveAction(outFile);
	delete temp;
}

void Scheduler::LoadAction(string file)
{
	ifstream inf;               
	
	inf.open(file);       //opening the file to start loading from it
	
	int NF, NR, NS, NE;       //implemented values NF for no. of FCFS and NS for no. of SJF and NR for no. of RR
	int RTF, MaxW, STL, FP; 
	
	int RT;               //implemented to get value of time slice for RR 

	inf >> NF >> NR >> NS>>NE;        //inserting values of no of each typr processor
	nf = NF; nr = NR; ns = NS;  //starting to save values in data members of class
	ne = NE;
	inf >> RT;							 //RT-->means time slice for round robin Alg
	numProcessor = NF + NR + NS+NE ;  //getting total number of processors will be useful later 
	
	if (nr > 0)
	{
		RTcheck = new int;
		*RTcheck = RT;
	}
	int OV, OVT;
	int p2 = NF + NR;             //getting value of tot FCFSs and RRs
	int p3 = p2 + ns;
	Processors = new Processor*[numProcessor]; //creating array of pointers of processors to put all type in same array and easily accessed
	Processor* PRSCR = NULL; //processor pointer to load any type of processor in 
	for (int i = 0; i < numProcessor; i++)  //looping to load each processor  
	{ 
		if (i>=0 && i < NF)         //if from zero to (FCFSs no.- 1) load FCFS 
		{
			PRSCR = new Processor_FCFS(this);
		}
		else if (NF <= i && i < p2) //if from no. of FCFSs to (RRs no. and FCFSs no. - 1) load RR
		{
			PRSCR = new Processor_RR(this,RT);
	    }
		else if (p2 <= i && i < p3)//if from RRs no. and FCFSs no. to (TotnumProcessor no.- 1) load SJF
		{
			PRSCR = new Processor_SJF(this);
		}
		else if (p3 <= i && i < numProcessor)
		{
			PRSCR = new Processor_EDF(this);
		}
		Processors[i] = PRSCR;
	}
	  
	inf >> RTF >> MaxW >> STL >> FP>>OV>>OVT;
	ohprop = OV;
	fp = FP;
	this->RTF = RTF;
	this->MaxW = MaxW;
	this->STL = STL;
	int n;
	inf >> n;                        //loading num of processes
	numProc = n;                     //setting it in scheduler data member (NOTE: assumes that input numProc is correct)
	for(int i=0;i<numProc;i++)
	{
		process* p = new process();  //implementing a new process to load in data of each process
		p->Load(inf);                //using load function of process it loads each process
		NEW.enqueue(p);              //putting it in NEW list of scheduler
		p->SetState("NEW");          //setting state of process to new
	}
	int killID, killTime;
	PriorityQueue<SigKill*>* KillList = new PriorityQueue<SigKill*>;
	while (inf >> killTime >> killID) // Load the ID and timestep of the process to be killed
	{
		SigKill* TempSig = new SigKill;
		TempSig->ID = killID;
		TempSig->timestep = killTime;
		// Add the SigKill object to the KillList
		KillList->enqueue(TempSig);
	}
	KillList->sortByTS();
	for (int i = 0; i < numProcessor; i++)
	{
		Processors[i]->InsertKillListPointer(KillList, fp);
	}
}

void Scheduler::SaveAction(ofstream& outf)
{
	outf << "TT" << " " << "PID" << " " << "CT" << " " << "AT" << " " << "IO_D" << " " << "WT" << " " << "RT" << " " << "TRT" << endl;
		Node<process*>* ptr;
		process* p;
		ptr = TRM.getHead();
		while (ptr)
		{
			p = ptr->getItem();
			p->Save(outf);
			ptr = ptr->getNext();

		}
		float rtfp=0, maxwp=0;
		if (MigRTF != 0 || MigMaxW != 0)
		{
			rtfp = MigRTF / (MigRTF + MigMaxW);
			maxwp = MigMaxW / (MigRTF + MigMaxW);
		}

	outf << "Processes: " << numProc<<endl;
	outf << "Avg WT: " << getTotalWT() / static_cast<float>(numProc) << ",    " << "Avg RT: " << getTotalRT() / static_cast<float>(numProc) << ",    " << "Avg TRT: " << getTotalTD() / static_cast<float>(numProc) << ",    " << endl;
	outf << "Migrations: "<<(MigRTF+MigMaxW)<<"		Migration%: " << (MigRTF + MigMaxW) / static_cast<float>(numProc) *100 << "%     " << "RTF: " <<  rtfp* 100 << "%,    " << "MaxW: " << maxwp * 100 << "%,    " << endl;
	outf << "Work Steal%: " << Stolen / static_cast<float>(numProc)* 100 << "%" << endl;
	outf << "Forked Processes%: " << Forked /static_cast<float>(numProc) * 100 << "%" << endl;
	outf << "Killed Processes%: " << Killed /static_cast<float>(numProc) * 100 << "%" << endl;
	outf << "Processors: " << numProcessor << " [" << nf << " FCFS," << ns << " SJF," << nr << " RR, " << ne << " EDF" << "]" << endl;
	outf << "Processors Load" << endl;
	for (int i = 0; i < numProcessor; i++) 
	{
		Processor* pc = Processors[i];
		outf << "P" << i+1 << "=" << pc->TotLoadCalc()*100 << "%,     ";

	}
	float avgUtil=0;
	outf << endl << "Processors utilization" << endl;
	for (int i = 0; i < numProcessor; i++)
	{
		Processor* pc=Processors[i];
		outf << "P" << i+1 << "=" << pc->UtilCalc()*100<< "%,     ";
		avgUtil += pc->UtilCalc();
	}
	avgUtil = avgUtil / numProcessor;
	outf << endl << "Avg  utilization = "<<avgUtil*100<<"%";
}


void Scheduler::NEWToRDY()
{
	
	bool h = true;
	Node<process*>* ptr = NEW.getHead();
	while (!NEW.isEmpty() && h == true) //loop works when NEW list not empty and h is true that will help to load more than one process has same timestep 
	{
		if (numProc >= 1)
			h = GetAts(ptr->getItem());
		else break;
		if (h == true) //starting to insert finally if h still true  
		{
			process* p = NEW.dequeue();
			int x = FindShortestQueue();
			Processors[x]->InsertToRDY(p);
			Processors[x]->RecalcExp("a", p);
			NEWnum++;
			if (numProc > 1)
				ptr = NEW.getHead();
	
		}

	}
}

int Scheduler::FindShortestQueue()
{
	int x=0;
	int min=Processors[0]->ExpectedTime();
	for (int j = 1; j < numProcessor ; j++)
	{
		if (Processors[j]->ExpectedTime() < min)
		{
			x = j;
			min = Processors[x]->ExpectedTime();
		}
	}
	return x;
}

int Scheduler::FindShortestFCFS()
{
	int x=-1;
	bool exists = false;
	int min=INT_MAX;
	if (dynamic_cast<Processor_FCFS*>(Processors[0]))
	{
		min = Processors[0]->ExpectedTime();
		exists = true;
		x = 0;
	}
	for (int j = 1; j < numProcessor; j++)
	{
		if ((Processors[j]->ExpectedTime() < min) && dynamic_cast<Processor_FCFS*>(Processors[j]))
		{
			x = j;
			min = Processors[x]->ExpectedTime();
			exists = true;
		}
	}
	if (exists)
		return x;
	else
		return -1;
}

int Scheduler::FindShortestRR()
{
	int x=-1;
	int min=INT_MAX;
	if (dynamic_cast<Processor_RR*>(Processors[0]))
	{
		min = Processors[0]->ExpectedTime();
		x = 0;
	}
	for (int j = 1; j < numProcessor; j++)
	{
		if ((Processors[j]->ExpectedTime() < min) && dynamic_cast<Processor_RR*>(Processors[j]))
		{
			x = j;
			min = Processors[x]->ExpectedTime();
			 
		}
	}
	 
		return x;
}


void Scheduler::IO_handling(process*p)
{
	RUNnum--;
	InsertBLK(p);
}

void Scheduler::migrationprocess_from_FCFS_To_RR(process* p)
{


	int x = FindShortestRR();
	Processors[x]->InsertToRDY(p);
	MigMaxW++;
}
Scheduler::Scheduler()  //Constructor
{
	Queue<process*> NEW;
	Queue<process*> TRM;
	Queue<process*> BLK;
	RTcheck = nullptr;
	TS = 1;
	BLKnum = 0;
	TRMnum = 0;

	RTF = 0; MaxW = 0;
	MigMaxW = 0;
	MigRTF = 0;
	Stolen = 0;
	numProc = 0;
}


int Scheduler::GetMaxW()
{
	
	
		return MaxW;
}
bool Scheduler::GetAts(process*p)  //checking on the arrival for each process
{
	int at;
	at=p->GetAT();
	if (at == TS)
		return true;
	else return false;

}



Scheduler::~Scheduler()
{
	while(!TRM.isEmpty())
	{
		delete TRM.dequeue();
	}
	for (int i = 0; i < numProcessor; i++)
	{
		delete Processors[i];
	}
	delete[] Processors;
	if (RTcheck!=nullptr)
		delete RTcheck;
}

int Scheduler::GetNUMProcessors()
{
	return numProcessor;
}

int Scheduler::CalcRUNnum()
{
	int c = 0;
	for (int i = 0; i < numProcessor; i++)
	{
		if (Processors[i]->Status())
			c++;
	}
	return c;
}

void Scheduler::InsertTRM(process* p) //function to insert in terminated queue
{
	TRM.enqueue(p);
	p->SetTT(TS);
	if (p->isRUN())
	RUNnum--;
	TRMnum++;
	p->SetState("TRM");
	
}

void Scheduler::InsertBLK(process* p) //function to insert in blocked queue
{

	BLK.enqueue(p);
	BLKnum++;
	p->SetState("BLK");
	
}

void Scheduler::printBLK() //print queue of blk
{
	cout << BLKnum << "  " << " BLK: ";
	BLK.print();
}

void Scheduler::printTRM() //print queue of trm
{
	cout << TRMnum << "  " << " TRM: ";
	TRM.print();
}

void Scheduler::printRUN()
{
	cout << RUNnum << "   " << "RUN: ";
	for (int i = 0; i < numProcessor; i++)
		if (Processors[i]->getRUN() != nullptr)
		{
			cout << "P" << i + 1 << "(" << Processors[i]->getRUN()->GetPID() << ")" << " ";
		}
}

void Scheduler::printRDY()
{
	for (int i = 0; i < numProcessor; i++)
	{
		int p2 = nf + nr; int p3 = p2 + ns;
		if (i >= 0 && i < nf)
		{
			cout << i + 1 << " " << "processor[FCFS]  " << ": " << Processors[i]->RDYnum << " RDY: ";
		}
		else if (nf <= i && i < p2)
		{
			cout << i + 1 << " " << "processor[RR]    " << ": " << Processors[i]->RDYnum << " RDY: ";
		}
		else if (p2 <= i && i < p3)
		{
			cout << i + 1 << " " << "processor[SJF]   " << ": " <<Processors[i]->RDYnum <<" RDY: ";
		}
		else if (p3 <= i && i < numProcessor)
		{
			cout << i + 1 << " " << "processor[EDF]   " << ": " << Processors[i]->RDYnum << " RDY: ";
		}
		Processors[i]->PrintRDY();
		cout << endl;
	}
}

void Scheduler::BLKToRDY()  //function to get out process from blk and send it to be inserted in rdy of any processor
{
	Node<process*>* h;
	int l;
	if (!BLK.isEmpty()) {
		h = BLK.getHead();
		l= h->getItem()->GetIO()->io_dur;
		l--;
		if (l == 0) 
		{
			h->getItem()->RemoveIO();
			process* p = BLK.dequeue();
			int x = FindShortestQueue();
			Processors[x]->InsertToRDY(p);
			Processors[x]->RecalcExp("a", p);
			BLKnum--;
			//Processors[x]->CalcExpectedTime();
		}
		else
		{
			h->getItem()->GetIO()->setIO_DUR(l);
		}
	}
}


void Scheduler::ForkProcess(process* p)
{
	//Insert randomization sequence here
	int range1 = 0;     //min. range
	int range2 = 100;   //max. range
	int random = (rand() % (range2 - range1 + 1)) + range1;
	process* child = new process(p, TS, p->GetPID() + random, p->GetCT() - p->GetCRT()); //Create new child process with parent pointer set to p, ID set as parent's id * random - random / 2
	//set AT as current timestep and set CT as remaining timesteps of parent
	p->setChild(child);		//Set child pointer of parent to child process, and set forkedOnce to true, so parent can't fork again.
	int shortest = FindShortestFCFS();		//Find shortest FCFS queue
	Processors[shortest]->InsertToRDY(child);	//Send to the RDY list of that processor
	numProc++;
	Forked++;
}

int* Scheduler::getForkProb()
{
	return &fp;
}

float Scheduler::getTotalTD()
{
	Node<process*>* ptr;
	int totalTD=0;
	ptr = TRM.getHead();
	while (ptr)
	{
		totalTD += ptr->getItem()->GetTD();
		ptr = ptr->getNext();

	}
	return totalTD;
}

float Scheduler::getTotalWT()
{
	Node<process*>* ptr;
	int totalWT = 0;
	ptr = TRM.getHead();
	while (ptr)
	{
		totalWT += ptr->getItem()->GetWT();
		ptr = ptr->getNext();

	}
	return totalWT;
}

float Scheduler::getTotalRT()
{
	Node<process*>* ptr;
	int totalRT = 0;
	ptr = TRM.getHead();
	while (ptr)
	{
		totalRT += ptr->getItem()->GetRT();
		ptr = ptr->getNext();

	}
	return totalRT;
}

int Scheduler::FindShortestSJF()
{
	int x=-1;
	int min = INT_MAX;
	for (int j = 1; j < numProcessor; j++)
	{
		if ((Processors[j]->ExpectedTime() < min) && dynamic_cast<Processor_SJF*>(Processors[j]))
		{
			x = j;
			min = Processors[x]->ExpectedTime();

		}
	}
	return x;
}
void Scheduler::MigrationFromRRToSJF(process* p) //print queue of trm
{

	int x = FindShortestSJF();
	Processors[x]->InsertToRDY(p);
	MigRTF++;

}
int Scheduler::GetRTF()   //returns current timestep of scheduler
{
	return RTF;
}
int Scheduler::FindLongestQueue()
{
	int x = 0;
	int max = Processors[0]->ExpectedTime();
	for (int j = 1; j < numProcessor; j++)
	{
		if (Processors[j]->ExpectedTime() > max)
		{
			x = j;
			max = Processors[x]->ExpectedTime();
		}
	}
	return x;
}
void Scheduler::WorkStealing()   //returns current timestep of scheduler
{
	for (int i = 0; i < numProcessor; i++) //loop to recalculate exptofinish
	{
		Processors[i]->CalcExpectedTime();
	}
	int x = FindLongestQueue();
	float LQF = Processors[x]->ExpectedTime();
	int y = FindShortestQueue();
	float SQF = Processors[y]->ExpectedTime();
	process* childCheck = nullptr;				//Used for a check if FCFS being stolen from
	while (((LQF - SQF) / LQF) > 0.4)
	{
		if (Processors[x]->RDYEmpty())
		{
			break;
		}
		else
		{
			process* prc = Processors[x]->GetProcessFromRdy();
			if (dynamic_cast<Processor_FCFS*>(Processors[x]))	//There is a side effect of the FCFS version of the above fn
				Killed--;	//It uses ProcKILL which is used in sigkill, as a side effect it increments the killed count. The check mitigates this
			if (prc->IsChild() && !dynamic_cast<Processor_FCFS*>(Processors[y]))	//If a non FCFS processor steals child, reverse the operation
			{
				Processors[FindShortestFCFS()]->InsertToRDY(prc);		//Return the stolen process and recalculate the time
				if (childCheck == prc)				//If the first child process is attempted to be stolen again (an infinite loop is happening)
					return;							//End the work steal sequence after returning the stolen process where it belongs
				if (!childCheck)					//If an attempt to steal a child for the first time occurs
					childCheck = prc;				//Store a pointer to it
			}
			else
			{
			Processors[y]->InsertToRDY(prc);
			LQF = Processors[x]->ExpectedTime();
			SQF = Processors[y]->ExpectedTime();
			Stolen++;
			}
		}
 
	}
}

void Scheduler::IncKilled()
{
	Killed++;
}

bool Scheduler :: SimOver()
{
	return (numProc == TRMnum);
}

int Scheduler::ErrorCheck()
{
	if (numProcessor == 0)		//Error checks
	{
		return 1;
	}
	else if (numProc == 0 && numProcessor != 0)
	{
		return 2;
	}
	else if (numProc == 0 && numProcessor == 0)
	{
		return 3;
	}
	else if (RTcheck && *RTcheck == 0)
	{
		return 4;
	}
	else if (RTcheck && *RTcheck < 0)
	{
		return 5;
	}
	if (RTcheck)
	{
		delete RTcheck;
		RTcheck = nullptr;
	}
	return 0;	//All clear
}

void Scheduler::UpdateRT(process* p)
{
	p->SetRT(TS);
}

int Scheduler::CalcTWT(process* p)
{
	return TS - p->GetAT() - p->GetCRT();
}

bool Scheduler::SigKillGreenlight(SigKill* s)	//Checks if it is time for SigKill to commence
{
	if (s->timestep <= TS)
		return true;
	else return false;
}