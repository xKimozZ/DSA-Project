#include "Processor_FCFS.h";

//void Processor_FCFS :: Execute(process* p)
//{
	//if (IncomingTasks.rear == IncomingTasks.front == -1)
	//	return;
	//IncomingTasks.rear->Execute();
	//IncomingTasks.pop();
//}

void Processor_FCFS::Enqueue(process p)
{
	ReadyTask.enqueue(p);
}

void Processor_FCFS::Load(ifstream& Infile)
{
	Processor* p = new Processor_FCFS;
}

void Processor_FCFS::Execute() {

	// Check if there are any processes in the queue and set as IDLE if empty
	if (ReadyTask.isEmpty())
	{
		setState(IDLE);
		return;
	}
	setState(BUSY); //Set as BUSY if execution starts

	// Get the first process in the queue
	process* p = new process;
	ReadyTask.dequeue(p);

	// execute the process
	//p->Execute(); Will need to figure out timestep
	enqExecuted(p); //Adds the executed task to the ExecutedTasks queue, if the memory needs to be freed afterwards, we can do it somehow through that queue.
   }