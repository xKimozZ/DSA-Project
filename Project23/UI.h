#ifndef USER_INTERFACE
#define USER_INTERFACE
#include<iostream>
#include"Scheduler.h"
#ifdef _unix_            
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#endif

using namespace std;
class UI
{
private:
	
public:
	int t;
	UI();
	void show(Scheduler *s, int ts);	//Print output and real-time data
	void Type();
	void Error(int code);	//Error messages
};
#endif
