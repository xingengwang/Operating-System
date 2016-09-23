//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include <time.h>
#include <stdio.h>
#include "commonWin.h"



int Square(int N)
{
	Square_called=Square_called+1;
	if (N == 0)
	{
		return (0);
	}
	else
	{
  		return (Square (N-1) + N + N - 1);
	}
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
