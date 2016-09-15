#include <time.h>
#include <stio.h>



int Square()
{
  printf("hello word!");
  return 0;
}

int getSysTime(int *returnTime)
{
  clock_t clockTime;s
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
