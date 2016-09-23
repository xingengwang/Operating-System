//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "commonWin.h"
#include <windows.h>
#include <stdio.h>

bool keepRunning;

DWORD WINAPI ThrdFunction(LPVOID lpParam)
{
	getSysTime(&returnTime);
	int start_time = returnTime;
	int end_time;
	int elapsedtime;
	int N = (int) lpParam;
	if(keepRunning == true)
	{
		printf("The Current Thread Id is： %d\n", GetCurrentThreadId());
		for(int i =0; i< N; i=i+1)
		{
			Square(i);			
		}
		  
		
	}
	else
	{
		getSysTime(&returnTime);
		end_time = returnTime;
		elapsedtime = end_time-start_time;
		printf("The Elapsed Time for Current Thread is： %d\n", elapsedtime);  	
		printf("The Square has been called by Current Thread： %d\n", Square_called);
		ExitThread(0);
	}
}
//int RUNNING=0;


int main()
{
	int THREAD,DEADLINE,SIZE;
	scanf( "%d", &THREAD );
	scanf( "%d", &DEADLINE );
	scanf( "%d", &SIZE );
	DWORD threadID;
	
	for(int i=0; i<THREAD; i=i+1)
	{
		HANDLE HANDLE_of_Thread = CreateThread(NULL, //security attributes(default if NULL)
									0, //stack size default if 0
									ThrdFunction,//start 
									(LPVOID)SIZE, //input data
									0, //creational flag(start if 0)
									&threadID
										);
		
	}

	Sleep(DEADLINE*1000);
	keepRunning= false;
	return 0;
}
