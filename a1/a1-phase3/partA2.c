//Xingeng Wang, xiw031, 11144515
//Yuchen Lin  , yul761, 11138672
#include "commonWin.h"

#include <stdio.h>
#include <standards.h>
#include <os.h>

PID pid_parent;
PID pidb;

PROCESS a(int sem)
{
    int times = TIMES;

    P(sem);
    printf("1 done P\n");
    V(sem);
    printf("1 done V\n");

    printf("a is sending a message to b \n");
    Send(pidb, NULL, 0);


    printf("a received a reply from b \n");

    while(times--)
	{
        Sleep(4);
	printf("fasttimo\n");
        }
}

void mainp()
{
   long sem;

    setbuf(stdout, 0);
    sem = NewSem(0);
    pid_parent = Create( (void(*)()) a, 16000, "a", (void *) sem, NORM, USR );
}