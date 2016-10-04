/*Xingeng Wang, xiw031, 11144515
Yuchen Lin  , yul761, 11138672*/
#include <stdio.h>
#include <standards.h>
#include<kernel.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

volatile int scalled;
int counter[500000000];


int Square(int N)
{	int ID = (int)(pthread_self());
     if(ID <0)
    {
   ID = 4294967295 + ID;
    }
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


void *child_thread(void *SIZE)
{
  struct timeval start, end;
   gettimeofday(&start, NULL);
 unsigned long threadid = (long)(pthread_self());

 if(threadid <0)
 {
   threadid = 4294967295 + threadid;
}
 
  int i;
  int *s;
  s=(int *)SIZE;
 for(i=0; i<= s ;i=i+1)
 {
    Square(i);
 }
  
  gettimeofday(&end, NULL);
  int timeused=(end.tv_usec-start.tv_usec)/1000;
  if(timeused<0)
  {
    timeused=timeused*(-1);
  }
  
  printf("child pid %u called square() %d times in %d ms\n",threadid, counter[threadid],timeused);

  pthread_exit();
}


int main(int argc, char *argv[])
{
  int THREAD,DEADLINE,SIZE;
  THREAD= atoi(argv[1]);
  DEADLINE= atoi(argv[2]);
  SIZE= atoi(argv[3]);
  
  pthread_t id[THREAD];

  int i,t;
  for(i=0; i<THREAD ; i=i+1)
  {
    
    t=pthread_create( &id[i], NULL, &child_thread, (void *)SIZE );
    if (t){
          printf("ERROR; return code from pthread_create() is %d\n", i);
          exit(-1);
       }
  }
  sleep(DEADLINE);
for(i=0; i<THREAD ; i=i+1)
  {
    pthread_kill(id[i],0);
  }
  

  return 0;
}
