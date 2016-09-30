//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "commonWin.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>


int keepRunning=1;
int returnTime;
int getSysTime(int *returnTime);

DWORD WINAPI ThrdFunction(LPVOID SIZE)
{
	getSysTime(&returnTime);
	int start_time = returnTime;
	int end_time;
	int elapsedtime;
	int N = (int) SIZE;
	
	int i;
	for(i=0; i<= N; i=i+1)
	{
	
		Square(i);
		if(!keepRunning)
		{
			break;
		}		
	}

	getSysTime(&returnTime);
	end_time = returnTime;
	elapsedtime = end_time-start_time;
	int ID=GetCurrentThreadId(); 	
	printf("Thread Id %d take %d ms call Square %d times.\n",  ID ,elapsedtime, counter[ID]);
	
	ExitThread(0);
	
}

int getSysTime(int *returnTime)
{
  clock_t clockTime;
  int currTimeInms;
  double milliseconds;
  
  clockTime = clock();
  milliseconds = ((double)clockTime / CLOCKS_PER_SEC)*1000;
  currTimeInms = (int) milliseconds;
  
  if(returnTime != NULL)
  {
    *returnTime = currTimeInms;
  }
  else
  {
    return -1;
  }
  
	return 0;
}


int main(int argc, char* argv[])
{
	int THREAD,DEADLINE,SIZE;

	THREAD= atoi(argv[1]);
	DEADLINE= atoi(argv[2]);
	SIZE= atoi(argv[3]);
	DWORD threadID;

	int i;
	for(i=0; i<THREAD; i=i+1)
	{
		CreateThread(NULL, //security attributes(default if NULL)
									0, //stack size default if 0
									ThrdFunction,//start 
									(LPVOID)SIZE, //input data
									0, //creational flag(start if 0)
									&threadID
										);
		
		
	}
	
	
	Sleep(1000*DEADLINE);
	keepRunning = 0;
	return 0;
}
