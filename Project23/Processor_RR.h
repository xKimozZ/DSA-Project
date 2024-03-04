#ifndef PRO_RR
#define PRO_RR
#include<iostream>
#include "Processor.h"
#include "process.h"
//#include "Queue.h"
class Processor_RR : public Processor
{
private:
	int RT;			//Fixed time slice for Round Robin
	Queue<process*> RDYTask;
public:
	int currentRslice;
	virtual void InsertToRDY(process* p);
	virtual float TotLoadCalc();
	virtual float UtilCalc();
	virtual void ScheduleAlgo();
	virtual bool RDYEmpty();
	virtual void RDYToRUN();
	virtual void PrintRDY();
	virtual void resetProcesses();
	virtual void CalcExpectedTime();
	virtual float ExpectedTime();
	void IncRUN();
	Processor_RR(Scheduler *s,int rt);
	int GetMaxW();
	process* GetProcessFromRdy();
};
#endif
