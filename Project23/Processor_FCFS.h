#ifndef PRO_FCFS
#define PRO_FCFS
#include"Processor.h"
#include"SigKill.h"
#include "LinkedList.h"

class Processor_FCFS:public Processor
{
private:
	LinkedList<process*> RDYTask;
	int* ForkProb;		//Probability of forking(PH2)
	PriorityQueue<SigKill*>* KillList;	//List to be checked at every timestep. Checks time and if the PID exists (Pointer to universal KillList in scheduler)
	process* SigKilled;		//Temporary pointer to store a process that was SigKilled if found
public:

	virtual void InsertToRDY(process* p);
	virtual float TotLoadCalc();
	virtual float UtilCalc();
	virtual void ScheduleAlgo();
	virtual bool RDYEmpty();
	virtual void RDYToRUN();
	virtual void PrintRDY();
	virtual void resetProcesses();
	virtual int retCurrentValidSigKill();
	process* GetProcessFromRdy();
	void IncRUN();					//Increment running process
	bool SignalKillCheck();	//Check if SigKill request's ID is found
	int getSigKillID();				//returns the first SigKill request's ID for simplicity
	process* retSigKilled();		//Returns SigKilled for scheduler
	void resetSK();					//Resets SigKilled after scheduler recieves it
	process* ProcKILL(int id);
	virtual process* runForkKILL(int id);
	virtual void InsertKillListPointer(PriorityQueue<SigKill*>* s,int fp);
	virtual void DeleteSigKill();
	Processor_FCFS(Scheduler* s);
	virtual void CalcExpectedTime();
	virtual float ExpectedTime();
};
#endif