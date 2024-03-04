#ifndef PROCESSOR
#define PROCESSOR
#include "process.h"
#include "SigKill.h"
//#include "Queue.h"
#include "PrioQueue.h"
#include<fstream>
#include <iostream>
//#include"Scheduler.h"
using namespace std;

class Scheduler;

class Processor
{
private:
    process* running;
	bool Busy;	//Busy or not?
	float BusyTime;		//Total time passed in BUSY state(PH2)
	float IdleTime;		//Total time passed in IDLE state(PH2)

public:
	int ORT; //overheat remaining time
	bool stop;      //(bonus) related to overheat 
	Scheduler* S;
	bool OverHeatedOrNot();
	int RDYnum;
    int ExpToFinish;    //(PH2)
	bool Status();	//Return true if busy
	bool overheated();
	void SetStatus(bool s);	//Set state
	void PrintRUN();
	process* getRUN();	//get running process
	void SetRUN(process* p);	//Set process as the running process
	void RemRUN();	//Set running process as null
	virtual void InsertToRDY(process* p) = 0;
	virtual void ScheduleAlgo()=0;	//Scheduling algorithm that depends on type of processor
	virtual bool RDYEmpty() = 0;
	virtual void RDYToRUN()=0;
	virtual void IncRDY();
	virtual void decRDY();
	virtual float UtilCalc();      //Return processor utilization (BusyTime/(BusyTime+IdleTime))
	virtual float TotLoadCalc();    //Return processor load (BusyTime/TotalTT)
	virtual void PrintRDY() = 0;
	virtual void resetProcesses() = 0; //Reset process change states to true. Called ONLY at the very beginning of timestep.
	virtual void CalcExpectedTime() = 0;
	virtual float ExpectedTime() = 0;
	virtual process* ProcKILL(int id); //FCFS exclusive, find process by ID, kill it, and return the pointer. Used in SigKill and forking
	virtual process* runForkKILL(int id); //FCFS exclusive, same as above but in forking only
	virtual bool SignalKillCheck();	//FCFS exclusive, sigkill
	virtual process* GetProcessFromRdy() = 0;
	virtual void InsertKillListPointer(PriorityQueue<SigKill*>* s, int fp);	//FCFS exclusive. Sets up pointer inside FCFS processor to universal killist and forkprob
	virtual int retCurrentValidSigKill();
	virtual void DeleteSigKill();
	Processor(Scheduler*s);
	Processor();
	~Processor();
	void IncIdle();
	void IncBusy();
	void RecalcExp(string op, process* p);		//Remove or add time as pleased. O(1) operation
	float	 GetIdle();
	float	 GetBusy();
	virtual void zeroExpT();
};
#endif