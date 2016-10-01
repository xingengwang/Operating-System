#include "kernel.h"

/* maybe want the number os semaphores to grow dynamically */

/* this module is responsible for process synchronization (but not IPC)       */

/* structure to describe a semaphore */
typedef struct
    {
    PID firstBlockedProc;
    PID lastBlockedProc;
    int semaphValue;
    int nextFreeSem;
    short inUse;
    } Semaphore;

static Semaphore semaphores[NUMBSEMAPHORES];
static int       firstFreeSem;

void initSynch()
{
    int semaphore;

    firstFreeSem = 0;
    for( semaphore = 0; semaphore < NUMBSEMAPHORES; semaphore++ )
        {
        semaphores[semaphore].firstBlockedProc = -1;
        semaphores[semaphore].lastBlockedProc  = -1;
        semaphores[semaphore].inUse            =  0;
	semaphores[semaphore].nextFreeSem      = semaphore + 1;
        }

    semaphores[NUMBSEMAPHORES - 1].nextFreeSem = -1;
}

/* load caller into list of processes blocked waiting on this semaphore       */
static void addBlockedProc( int semaph )
{
    if( semaphores[semaph].firstBlockedProc != -1 )
        {
        SetSemaphPrev( currp, semaphores[semaph].lastBlockedProc);
        SetSemaphNext( getPcbFromPid(semaphores[semaph].lastBlockedProc), 
                        GetPid(currp) );
        }
    else    /* no one is currently blocked on this semaphore                  */
        {
        semaphores[semaph].firstBlockedProc = GetPid( currp );
        SetSemaphPrev( currp, -1 );
        }

    SetSemaphore( currp, semaph );
    SetSemaphNext( currp, -1 );
    semaphores[semaph].lastBlockedProc = GetPid( currp );
}

/* take semaphore blocked process off the semaphore list                      */
static void dequeueBlockedProc( PID pid )
{
    PID   prev, next;
    PNODE *thisPcb;
    int   semaph;

    thisPcb = getPcbFromPid( pid );
    prev    = GetSemaphPrev( thisPcb );
    next    = GetSemaphNext( thisPcb );
    semaph  = GetSemaphore(  thisPcb );

    if( prev == -1 )
        semaphores[semaph].firstBlockedProc = next;
    else
        SetSemaphNext( getPcbFromPid( prev ), next );

    if( next == -1 )
        semaphores[semaph].lastBlockedProc = prev;
    else
        SetSemaphPrev( getPcbFromPid( next ), prev );

    SetSemaphNext( thisPcb, -1 );
    SetSemaphPrev( thisPcb, -1 );
    SetSemaphore(  thisPcb, -1 );
}

/* to be called for every process leaving the system                          */
void synchProcCleanup( PID pid )
{
    if( GetSemaphore( getPcbFromPid( pid ))  != -1 )
        {
        PNODE *blockedPcb;
        int   semaphore;

        blockedPcb = getPcbFromPid( pid );
        semaphore  = GetSemaphore( blockedPcb );

        /* take the semaphore off its list */
        dequeueBlockedProc( pid );

        /* increment semaphore value to reflect one fewer blocked processes   */
        semaphores[semaphore].semaphValue++;
        }
}

PUBLIC int NewSem(int value)
{
    int retVal;

    ENTERINGOS;
    if( firstFreeSem == -1 )
	{
	printf("NewSem: out of semaphores!!\n");
	retVal = -1;
        }

    else
	{
	retVal = firstFreeSem;
	semaphores[firstFreeSem].inUse = 1;
	semaphores[firstFreeSem].semaphValue = value;

	firstFreeSem = semaphores[firstFreeSem].nextFreeSem;
	}

    LEAVINGOS;

    return(retVal);
}

PUBLIC void FreeSem(int semno)
{
    ENTERINGOS;
    if( SEMAPHINRANGE( semno ) )
	{
        int readiedProcs = 0;
	while(semaphores[semno].firstBlockedProc != -1)
            {
            PID proc;
            PNODE *procPcb;

            proc = semaphores[semno].firstBlockedProc;
            procPcb = findPidOnQ( SEMB, proc );

            dequeueBlockedProc( proc );  /* take off semaphore list */

            /* make unblocked process ready (put on ready queue)    */
            SetState( procPcb, READYQ( procPcb ) );
            DEQPPTR( SEMB, procPcb );
            ENQ(GetState(procPcb), procPcb);
            readiedProcs++;
            }

	semaphores[semno].inUse = 0;
	semaphores[semno].nextFreeSem = firstFreeSem;
	firstFreeSem = semno;

        if(readiedProcs)
	    printf("FreeSem: sem had %d blocked processes\n", readiedProcs);
	}
    LEAVINGOS;
}


PUBLIC int P( int semaphore )
{
    int retVal;
    short doCtxtSw = 0;

    ENTERINGOS;

    if( SEMAPHINRANGE(semaphore) )
        {
        retVal = 0;
        
        if( (--(semaphores[semaphore].semaphValue)) < 0 )
            {
            doCtxtSw = 1;
            addBlockedProc( semaphore );
            SetState( currp, SEMB );
            }
        }
    else
        retVal = -1;

    if( doCtxtSw )
        SchedM;

    LEAVINGOS;

    return( retVal );
}

PUBLIC int V( int semaphore )
{
    int retVal;
    short doCtxtSw = 0;

    ENTERINGOS;

    if( SEMAPHINRANGE(semaphore) )
        {
        retVal = 0;

        if( (++(semaphores[semaphore].semaphValue)) <= 0 )
            {
            PID proc;
            PNODE *procPcb;

            doCtxtSw = 1;
            proc = semaphores[semaphore].firstBlockedProc;
            procPcb = findPidOnQ( SEMB, proc );

            dequeueBlockedProc( proc );  /* take off semaphore list */

            /* make unblocked process ready (put on ready queue)    */
            SetState( procPcb, READYQ( procPcb ) );
            DEQPPTR( SEMB, procPcb );
            ENQ(GetState(procPcb), procPcb);
            }
        }
    else
        retVal = -1;

    if( doCtxtSw )
        SchedM;

    LEAVINGOS;

    return( retVal );
}

/* returns 0 on success, -1 on failure */
PUBLIC int GetSemVal( int semaphore, int *value )
{
    int retVal;

    ENTERINGOS;

    if( SEMAPHINRANGE(semaphore) )
	{
	*value = semaphores[semaphore].semaphValue;
	retVal = 0;
	}
    else
	retVal = -1;
	
    LEAVINGOS;

    return( retVal );
}

/* this initializes the semaphore value. It is very dangerous to call this at */
/* any time after the semaphore has been used.                                */
PUBLIC int InitSemaph( int semaphore, int value )
{
    int retVal;

    ENTERINGOS;
    
    if( SEMAPHINRANGE(semaphore) )
        {
        semaphores[semaphore].semaphValue = value;
        retVal = 0;
        }
    else
        retVal = -1;

    LEAVINGOS;

    return( retVal );
}
