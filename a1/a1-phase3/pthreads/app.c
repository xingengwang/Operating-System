#include <stdio.h>

#define TIMES  0xf

#include <standards.h>
#include <os.h>

PID pida;
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

PROCESS b(int sem)
{
    int times = TIMES;
    int len;
    PID pid;


    printf(" in 2\n");
    V(sem);
    printf("2 done V\n");
    P(sem);
    printf("2 done P\n");

    Receive(&pid, &len);    /* await a message from a */
    printf("b received a message from a \n");

    Reply( pid, NULL, 0 );
    printf("b replied to a\n");

    while(times--)
	{
        Sleep(10);
	printf("              slowtimo\n");
	}
}

void mainp()
{
    long sem;

    setbuf(stdout, 0);
    sem = NewSem(0);
    pida = Create( (void(*)()) a, 16000, "a", (void *) sem, NORM, USR );
    pidb = Create( (void(*)()) b, 16000, "b", (void *) sem, NORM, USR );
    printf("processes created\n");
}
