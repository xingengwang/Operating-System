//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672

#include <time.h>
#include <stdio.h>



int Square()
{
  printf("hello word!\n");
  return 0;
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
  

}
