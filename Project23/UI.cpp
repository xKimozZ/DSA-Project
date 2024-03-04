
#include "UI.h"
#include<string>



UI::UI()
{
}

void UI::show(Scheduler* s, int ts)
{

	if (t != 3)
	{
	system("cls");
	cout << endl << "Current Timestep:" << ts << endl;
	cout << "-------------------    RDY Processes  --------------------" << endl;
	s->printRDY();
	cout << endl << "-------------------    RUN Processes  --------------------" << endl;

	s->printRUN();

	cout << endl << "-------------------    BLK Processes  --------------------" << endl;
	s->printBLK();

	cout << endl << "-------------------    TRM Processes  --------------------" << endl;
	s->printTRM();

	cout << endl;
	}
	if (s->SimOver())
	{
		cout << endl << "Simulation ends, Output file created" << endl;
		system("pause > nul");
		return;
	}

	 if (t == 2)
	{
		Sleep(1000);
	}

	else if (t == 1)
	{
		cout << "PRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
		system("pause > nul");

	}
}


void UI::Type()
{
	cout << "Choose type:" << endl;
	cout << "1) Interactive mode" << endl;
	cout << "2) Step by Step mode" << endl;
	cout << "3) Silent mode" << endl;
	int x;
	cin >> x;
	t = x;
}

void UI::Error(int code)	//Error messages
{
	system("cls");			//No errors
	if (code == 0)
		return;
	if (code == 1)
	{
	cout << "-------------------    WARNING!!!  -------------------- "<<endl<<"-------------------  Error code: 0 --------------------" << endl;
	cout << "No processors were specified in the input file." << endl << "The application cannot run." << endl;
	cout << "Please recheck the input file and ensure that non-zero processors are specified." << endl;
	cout << "Press any key to quit the application." << endl;
	system("pause >nul");
	}
	if (code == 2)
	{
		cout << "-------------------    WARNING!!!  -------------------- " << endl << "-------------------  Error code: 1 --------------------" << endl;
		cout << "No processes were specified in the input file." << endl << "The application cannot run." << endl;
		cout << "Please recheck the input file and ensure that non-zero processes are specified." << endl;
		cout << "Press any key to quit the application." << endl;
		system("pause >nul");
	}
	else if (code == 3)
	{
		cout << "-------------------    WARNING!!!  -------------------- " << endl << "-------------------  Error code: 2 --------------------" << endl;
		cout << "No processors and processes were specified in the input file." << endl << "The application cannot run." << endl;
		cout << "Please recheck the input file and ensure that non-zero processors and processes are specified." << endl;
		cout << "Press any key to quit the application." << endl;
		system("pause >nul");
	}
	else if (code == 4)
	{
		cout << "-------------------    WARNING!!!  -------------------- " << endl << "-------------------  Error code: 3 --------------------" << endl;
		cout << "Round robin processors were specified, but the time slice is zero." << endl << "The application cannot run." << endl;
		cout << "Please recheck the input file and ensure that time slice is a positive integer." << endl;
		cout << "Press any key to quit the application." << endl;
		system("pause >nul");
	}
	else if (code == 5)
	{
		cout << "-------------------    WARNING!!!  -------------------- " << endl << "-------------------  Error code: 4 --------------------" << endl;
		cout << "Round robin processors were specified, but the time slice is negative." << endl << "The application cannot run." << endl;
		cout << "Please recheck the input file and ensure that time slice is a positive integer." << endl;
		cout << "Press any key to quit the application." << endl;
		system("pause >nul");
	}
}