#ifndef PROCESS
#define PROCESS
#include"IO_REQ.h"
#include"Queue.h"
#include"Node.h"
#include"PrioQueue.h"
#include <fstream>
using namespace std;
class process
{
private:
	enum state { NEW, RDY, RUN, BLK, TRM, ORPH };
	state ST;			//(STATE) Process state
	int PID;			//(PROCESS_ID) Unique ID for each process
	int AT;				//(ARRIVAL_TIME) Arrival time of process
	int CT;				//(CPU_TIME) Timesteps process will last in RUN state, doesn't include IO time.
	int RT;				//(RESPONSE_TIME) Time between arrival and first time process is RUN
	int TT;				//(TERMINATION_TIME) Time process reaches TRM state
	int TD;             //(TURNAROUND_DURATION) Total time the process spent from its arrival to its termination 
	int WT;             //(WAITING_TIME) Time spent br process in the system not being executed by CPU
	int CRT;			//(CURRENT_REMAINING_TIMESTEPS) Timesteps process already passed out of the CT time
	int NR;				//(NUMBER_REQUESTS) Number of I/O requests to be made by process in its lifetime
	int DL;
	int IO_D;			//(IO_DURATION) Stores actual time spend in IO during process execution
	PriorityQueue<IO_REQ*> r;    //(IO_REQUEST) Stores I/O request times and I/O duration times
	bool IsBLK;			//Process is in BLK state? (I/O)
	bool ChangeOK;		//Toggle allowing of list change if it happened once in timestep (TRUE if okay to change FALSE if not)
	int  rec_retDeepestID(process* p);	//Returns the ID of the last child / grandchild
	bool forkedOnce;	//Did the process fork once? If yes, don't allow process to fork again //If now forked twice, then it is true now
	process* parent;
	process* child;
	process* child1;	//Second child

public:

	process(process* par, int at, int id, int ct);			//Constructor for child processes
	process();
	~process();
	void Load(ifstream& inf); //Load info for process objects
	void Save(ofstream& outf);
	int GetNum_IO();
	IO_REQ* GetIO();
	int GetAT();
	int GetPID();
	void SetPID(int id);
	void SetState(string s);
	void SetRT(int t);
	int GetRT();
	int GetCT();
	int GetCRT();
	int GetDL();
	int GetTD();
	int GetWT();
	int GetTT();
	void RemoveIO();
	void SetTT(int t);
	void ToggleChange();
	bool isChange();		//Returns if process is ready to change lists
	void setChild(process* nchild);					  //Sets child pointer and sets forkedOnce as true
	void calcFin(int t);	//Records the time the process has finally terminated
	void entered(int t);	//Calculates RT by passing the current timestep count, RT = t - AT
	void incCRT();			//increment the current timesteps
	bool isTRM();
	bool isRUN();
	int	 retDeepestID();
	bool OrphStillExists();
	bool ProcessForkAttempt(int ForkProb);			 //Calculates a random number and see if it falls within the probability of forking
	bool IsForked();
	bool IsChild();
	int returnIO_D();
};
#endif

