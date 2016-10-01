#include "kernel.h"

PNODE *currp;    /* process currently running                  */

/* the process table is a dynamically allocated array initially*/
/* of size INITPROCTABSIZE. Each time the table fills, it will */
/* be reallocated with space for PROCTABSIZEINCR more processes*/

short currProcTabSize;  /* current size of the process table   */
PNODE *procTable;       /* starting address of the proc table  */

int numbProcs = 0; /* number of existing processes */

/****************************************************************************/
/* the following are utility routines for QUEUE searching - etc.            */


PUBLIC PID MyPid()          /* return current process' pid */
{
    PID retVal;

    ENTERINGOS;

    retVal = GetPid( currp );

    LEAVINGOS;

    return( retVal );
}

PUBLIC PID NameToPid (char *name)  /* get pid, unless there is no such name */
{
    PID proc;
    PID retVal = -1;

    ENTERINGOS;

    /* search through process table looking for name "name" */
    for( proc = 0; proc < currProcTabSize; proc++ )
        {
        if( GetInUse( getPcbFromPid( proc ) ) &&
            (! (strcmp(GetPname(getPcbFromPid( proc ) ), name))) )
            {
            retVal = GetPid(getPcbFromPid(proc));
            break;
            }
        }

    LEAVINGOS;

    return(retVal);
}


/* the following provides a public interface to pExists */
PUBLIC int PExists( PID pid )
{
    int retVal;

    ENTERINGOS;

    retVal = pExists( pid );

    LEAVINGOS;

    return( retVal );
}


/* Suspend calling process.                                                  */
PUBLIC void Suspend()
{
    ENTERINGOS;

    SetState( currp, SUSP );

    SchedM;

    LEAVINGOS;
}


/* resume previously suspended process with PID pid. Returns 0 on success,   */
/* or -1 if process is not found.                                            */
PUBLIC int Resume( PID pid )
{
    PNODE *procToResume;
    int   retVal;

    ENTERINGOS;

    procToResume = findPidOnQ( SUSP, pid );

    if( procToResume )
        {
        /* remove process from suspended queue                  */
        DEQPPTR(SUSP, procToResume);

        /* find appropriate ready queue and place process on it */
        SetState( procToResume, READYQ(procToResume) );
        ENQ(GetState( procToResume ), procToResume);

        retVal = 0;
        }
    else
        retVal = -1;

    /* we must not do a ShedM here as this may be called in protected code.  */
    /* yes - I realize that a higher priority process may have to wait a bit.*/

    LEAVINGOS;

    return( retVal );
}

void procIsLeaving( PID pid )
{
    /* call all routines charged with cleaning up when a process leaves */

    ipcProcCleanup( pid );  /* clean up IPC related details.            */
    timeProcCleanup( pid ); /* clean up time related details.           */
    synchProcCleanup( pid );/* clean up synchronization related details.*/
    memProcCleanup( pid );  /* clean up memory management stuff.        */
}


PUBLIC PID Kill (PID pid)
/* returns pid of killed process, otherwise, 0                  */
{
    PNODE *dead;
    PID   deadPid;

    ENTERINGOS;

    if( (dead = getPcbFromPid( pid )) )
        {
        if( GetLevel(dead) == USR )
            numbProcs--;

        deadPid = pid;

        /* do clean-up tasks here, eg. inform send-blocked processes ... */
        procIsLeaving( deadPid );

        DEQPPTR(GetState( dead ), dead); /* take process off its queue   */
        SetInUse( dead, 0 );             /* this PCB is free for re-use. */
        }

    else
        deadPid = -1;

    SchedM;    /* in case of preemption */

    LEAVINGOS;

    return( deadPid );
}

PUBLIC void Pexit()
{

    ENTERINGOS;

    if( GetLevel(currp) == USR )
        numbProcs--;

    /* do clean-up tasks here, eg. inform send-blocked processes ... */
    procIsLeaving( GetPid( currp ) );

    SetInUse( currp, 0 );            /* this PCB is free for re-use. */

    /* this process now done, go ahead and schedule another */
    if( numbProcs )  /* if there are any user processes left */
        runNextProc();
    else    /* there are no more left to run - quit */
        {
        printf("\nexiting threads\n");
        exit(0);
        }
}

/* starts new process, and cleans up after it is done */
void callNclean()
{
    LEAVINGOS;  /* about to enter a user process.            */

    /* enter the process...                                  */
    (*currp->procMgmtInfo.proc_addr)(currp->procMgmtInfo.proc_arg);

    ENTERINGOS; /* back in threads.                          */

    if( GetLevel(currp) == USR )
        numbProcs--;/* one fewer process in the system.          */

    /* do clean-up tasks here, eg. inform send-blocked processes ... */
    procIsLeaving( GetPid( currp ) );

    SetInUse( currp, 0 );            /* this PCB is free for re-use. */

    /* this process now done, go ahead and schedule another  */
    if( numbProcs )  /* if there are any user processes left */
        runNextProc();
    else    /* there are no more left to run - quit */
        {
        printf("\nexiting threads\n");
        exit(0);
        }
}

/* initialize the PCBs with PIDs in the range first to last inclusive */
static void initPCBs( PID first, PID last )
{
    PID   proc;
    PNODE *pcb;

    for( proc = first; proc <= last; proc++ )
        {
        pcb = getPcbFromPid( proc );

        memset(   pcb,0, sizeof( PNODE ) );
        SetPid(  pcb, proc );

        /* intiialize interprocess communication PCB fields   */
        IPCPCBINIT( pcb );

        /* initialize queue management PCB fields.            */
        QMGMTPCBINIT( pcb );

        /* initialize scheduling PCB fields                   */
        SHEDULINGINIT( pcb );

        /* initialize time management PCB fields              */
        TIMEPCBINIT( pcb );

        /* initialize the process synchronization fields      */
        SYNCHPCBINIT( pcb );

        /* initialize the memory management fields            */
        MEMPCBINIT( pcb );
        }
}

/* a startup routine that creates the initial process control block table  */
void makeInitialPcbs()
{
    currProcTabSize = INITPROCTABSIZE;
    procTable       = (PNODE *) malloc( currProcTabSize * sizeof(PNODE) );
    initPCBs( 0, currProcTabSize - 1 );
}

static PID findUnusedPid( short *tabSize )
{
    PID pid;

    /* find a free process control block...                             */
    pid = 0;
    while( pid < *tabSize )
        {
        if( GetInUse( getPcbFromPid(pid) ) )
            pid++;
        else
            break;
        }

    if( pid == *tabSize )  /* table is full - allocate more room         */
        {
        if( PROCTABSIZEINCR <= 0 )/* fail if config doesn't allow growth */
            pid = -1;

        else
            {
            PID currpid;  /* pid of running process.                     */
            PID first;    /* first new PCB                               */
            PID last;     /* last new PCB                                */

            /* must be careful to reset currp after realloc              */
            if( currp )
                currpid = GetPid( currp );
    
            first     = *tabSize;
            *tabSize += PROCTABSIZEINCR;
            last      = *tabSize - 1;
    
            procTable = (PNODE*)realloc(procTable,((*tabSize) * sizeof(PNODE)));
    
            /* restore currp as it will have likely moved.               */
            if( currp )
                currp = getPcbFromPid( currpid );
        
            /* initialize these new PCBs.                                */
            initPCBs( first, last );

            pid = first;
            }
        } 

    return( pid );
}


/* create a process */
PUBLIC PID Create(void(*addr)(), int stksize, char *name, void *arg, 
	PPRIO prio, int level)
{
    /* create a new process */
  /* int *sp; */
    PNODE *pd;
    PID   pid;
    /* Kept these variables for historical reasons. Don't know why they
       were there in the first place DJM 2008/10/08 */
    /*    PID   currpid;
	  PID   first, last; */

    ENTERINGOS;

    pid = findUnusedPid( &currProcTabSize );

    if( pid != -1 )
        {
        /* at this stage we have found an unused PCB - mark it as being used */
        pd = getPcbFromPid( pid );
        SetInUse( pd, 1 );
        SetLevel( pd, level );
        
        SetPid( pd, pid );

        /* if the last stack is not the correct size, allocate a new one     */
        if( GetStackSize( pd ) != stksize )
            {
            if( GetStmembeg( pd ) ) 
                free( GetStmembeg( pd ) );

            SetStmembeg( pd, (int *) malloc(stksize) );
            SetStackSize( pd, stksize );
            }

        /* store process name, note - no duplicate name checking (on purpose) */
        if (name)
            {
            strncpy(GetPname( pd ), name, MAXPNAMELEN);
            GetPname( pd )[MAXPNAMELEN] = (char) 0;
            }
        else
            strcpy(GetPname( pd ), "nameless");

        /* set fields concerned with process management.                      */
        SETUPPROCMGMT( pd, prio, stksize, addr, arg );
    
        /* set fields concerned with interprocess communication.              */
        SETUPIPC( pd );

        /* call the routine to set up scheduling information.                 */
        SETUPSCHED(pd, DEFAULTQUANTUM); /* quantum length in clock ticks      */
    
	if( level == USR )
	    numbProcs++;
        ENQ(GetState( pd ), pd);
        }

    if( currp )    /* if we are in OS - not in mainp                          */
        SchedM;

    LEAVINGOS;

    return( pid );
}
