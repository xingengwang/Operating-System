//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include <windows.h>
#include <stdio.h>
#include "commonWin.h"



int Square(int N)
{
	int ID=GetCurrentThreadId();
	counter[ID]=counter[ID]+1; 
	if (N == 0)
	{
		return (0);
	}
	else
	{
  		return (Square (N-1) + N + N - 1);
	}
}


