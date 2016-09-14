//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

//#include "common.h"
#include <time.h>
#include <stio.h>


int getSystime(int *returnTime)

//parent thread

void main(int)
{
	HANDLE HANDLE_of_Thread = NULL:
	
	create_Thread(Handle_of_Thread);
	
	CloseHandle(HANDLE_of_Thread);

}



int Square(int N)
{
	if (N==0) return (0);
	return (Square(N-1)+N+N-1);
}



void _stdcall ChildThread(int x)
{
	printf("child thread created.\n");
	
}



void create_Thread(HANDLE Handle_of_Thread)
{
	
	DWORD threadID;
	HANDLE_of_Thread = CreateThread(NULL, //security attributes(default if NULL)
									0, //stack size default if 0
									ChildThread,//start 
									NULL, //input data
									0, //creational flag(start if 0)
									&threadID
										);
										
}
