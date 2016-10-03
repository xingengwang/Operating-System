/*Xingeng Wang, xiw031, 11144515
Yuchen Lin  , yul761, 11138672*/
#include "common.h"
#include <stdio.h>
#include <standards.h>
#include <os.h>
#include<kernel.h>
#include <stdlib.h>
#include <pthread.h>


volatile int THREAD,DEADLINE,SIZE;

void *child_thread(void *SIZE)
{
  int i;
  for(i=0; i<= SIZE ;i=i+1)
  {
     Square(i);
  }
  printf("child pid %d called square() %d times in %d ms\n",, counter[&child],5);
}

 void *parent_thread(void *THREAD)
 {
	int i;
	pthread_t child;
  for(i=0; i< THREAD ;i=i+1)
  {
    pthread_create( &child, NULL, child_thread, (void *)SIZE );
  }
  printf("Child: creating thread\n");  
    pthread_exit((void*) t);
 }


int main (int argc, char *argv[])
{
     
  THREAD= atoi(argv[1]);
  DEADLINE= atoi(argv[2]);
  SIZE= atoi(argv[3]);
  
  pthread_t parent;
  int mother;
  mother = pthread_create( &parent, NULL, parent_thread, (void *)THREAD );
  
  printf("Main: creating thread\n");    
  pthread_exit(NULL);
}