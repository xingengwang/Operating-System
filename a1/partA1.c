//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include "commonWin.h"
#include <windows.h>
#include <stdio.h>


//parent thread

//DWORD WINAPI ThrdFunction(LPVOID lpParam);
//int RUNNING=0;


void main()
{
	HANDLE HANDLE_of_Thread = NULL:
	DWORD threadID;
	HANDLE_of_Thread = CreateThread(NULL, //security attributes(default if NULL)
									0, //stack size default if 0
									Square,//start 
									NULL, //input data
									0, //creational flag(start if 0)
									&threadID
										);
	
	CloseHandle(HANDLE_of_Thread);

}
