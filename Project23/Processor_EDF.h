#ifndef PRO_EDF
#define PRO_EDF
#include"Processor.h"
#include"PrioQueue.h"
class Processor_EDF :public Processor
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
	process* GetProcessFromRdy();
	Processor_EDF(Scheduler* s);
};
#endif

