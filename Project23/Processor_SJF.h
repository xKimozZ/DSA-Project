#ifndef PRO_SJF
#define PRO_SJF
#include "Processor.h"
#include "process.h"
#include "PrioQueue.h"
class Processor_SJF : public Processor
{
private:
	PriorityQueue<process*> RDYTask;
public:

	virtual void InsertToRDY(process* p);
	virtual float TotLoadCalc();
	virtual float UtilCalc();
	virtual void ScheduleAlgo();
	virtual bool RDYEmpty();
	virtual void RDYToRUN();
	virtual void PrintRDY();
	void IncRUN();
	virtual void resetProcesses();
	virtual void CalcExpectedTime();
	virtual float ExpectedTime();
	Processor_SJF(Scheduler* s);
	process* GetProcessFromRdy();
};
#endif