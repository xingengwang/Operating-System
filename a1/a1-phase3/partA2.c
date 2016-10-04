/*Xingeng Wang, xiw031, 11144515
Yuchen Lin  , yul761, 11138672*/
#include "common.h"
#include <stdio.h>
#include <standards.h>
#include <os.h>
#include <sys/time.h>


volatile int THREAD,DEADLINE,SIZE;
volatile int stillrun[100];

PROCESS child(int SIZE)
{ 
  struct timeval start, end;
   gettimeofday(&start, NULL);
  float et;
  int i;
  for(i=0; i<= SIZE ;i=i+1)
  {
     Square(i);
  }
  gettimeofday(&end, NULL);
  int timeused=(end.tv_usec-start.tv_usec)/1000;
    if(timeused<0)
  {
    timeused=timeused*(-1);
  }
  stillrun[MyPid()]=1;
  printf("child pid %d called square() %d times in %d ms\n",MyPid(), counter[MyPid()],timeused);
}



PROCESS parent(int THREAD)
{
  int i;
  PID Childthread[THREAD];
  for(i=0; i< THREAD ;i=i+1)
  {
    Childthread[i]=Create( (void(*)()) child, 10000000, "child", (void *) SIZE, NORM, USR );
  }
  sleep(DEADLINE);
  for(i=2; i<THREAD+2;i++)
  {
    if(stillrun[i])
    {
    Kill(i);
    }
  }
  Pexit();
}




void mainp(int argc, char* argv[])
{
   
  THREAD= atoi(argv[1]);
  DEADLINE= atoi(argv[2]);
  SIZE= atoi(argv[3]);

  Create( (void(*)()) parent, 16000, "parent", (void *) THREAD, NORM, USR );
}
