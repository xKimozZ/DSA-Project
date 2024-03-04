#ifndef SCHEDULER
#define SCHEDULER
#include<fstream>
#include<cstdlib>
#include"process.h"
#include"Processor_FCFS.h"
#include"Processor_SJF.h"
#include"Processor_RR.h"
#include"Processor_EDF.h"
#include<string>

class Scheduler
{
private:
	Queue<process*> TRM;
	Queue<process*> BLK;
	Queue<process*> NEW;
	string filename;
	int BLKnum;	//IO processes
	int NEWnum;
	int TRMnum; //Number of terminated processes
	int RUNnum;	//Running processes
	int* RTcheck;		//Temporary timeslice integer for error checking
	int numProc;		//Processes number
	int numProcessor;	//Processor number
	int nf, nr, ns, ne; //No of FCFS ; RR ; SJF processors
	
	int TS;				//Timestep count
	int STL;			//Time at which process stealing occurs(PH2)
	float StealLimit; //Limit which if above, then work stealing can occur between shortest and largest queue
	int fp;				//Fork probability
	Processor** Processors;
	int RTF;			//(REMAINING_TIME_TO_FINISH) Threshold which if remaining time is lower than, then it should move from RR to SJF(PH2)
	int MaxW;			//(MAXIMUM_WAITING_TIME) If total waiting time is larger than this threshold, then FCFS queue process should move to RR(PH2)
	int minID, maxID;	//Used temporarily for phase 1 random ID generation, range.
	float MigMaxW;
	float MigRTF;
	float Stolen;
	float Killed;
	float Forked;
public:
	int ohprop;          //overheat probability
	int oht;              //overheat time
	void sendtoShortest(process* p);  //finds shortest queue and put process in it
	void sendtoNEW(process* p);        //same as previous but with new queue
	void killOrph(process* p);		//Kill all ORPH processes under a process once it terminates. FCFS exclusive 
	void simulate(string file);
	void LoadAction(string file);	//Load a file and update the members appropriately
	void SaveAction(ofstream& outf);  //getting output and final statistics
	void NEWToRDY(); //distribution of processes when it arrive on rdy of processors
    int FindShortestQueue();  //finds value of processor in the array that has shortest queue 
	int FindLongestQueue();    //same as previous but getting longest one
	int FindShortestFCFS();  //same but among FCFS
	int FindShortestSJF();      //same but among SJF
	int FindShortestRR();      // same but among RR
	void IO_handling(process*p);  //GET PROCESS TO BLK QUEUE
	void MigrationFromRRToSJF(process* p);  
	void migrationprocess_from_FCFS_To_RR(process* p);
	Scheduler();
	~Scheduler();
	int GetNUMProcessors();
	int GetMaxW();
	int GetRTF();
	void WorkStealing();
	bool GetAts(process* p);		//Check if process has arrived
	int CalcRUNnum();
	void InsertTRM(process* p);		//Insert process in TRM list
	void InsertBLK(process* p);		//Insert process in BLK list
	void printBLK();
	void printTRM();
	void printRUN();
	void printRDY();
	void BLKToRDY();			//Return process from BLK
	void ForkProcess(process* p);	//Creates child process, sets up the pointers, and sends the child process to the shortest queue available
	int* getForkProb();				//Return fork probability as pointer (for memory conservation)
	float  getTotalTD();
	float  getTotalRT();
	float  getTotalWT();
	bool SimOver();
	void IncKilled();
	int ErrorCheck();			//Check severity of errors and end application if necessary
	void UpdateRT(process* p);
	int	 CalcTWT(process* p);
	bool SigKillGreenlight(SigKill* s);		//Checks if it is time for SigKill to commence
};
#endif

