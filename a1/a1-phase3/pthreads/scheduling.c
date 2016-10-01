/* UBC pthreads
   scheduling.c
*/
#include "kernel.h"
#include <unistd.h>

EXTERN void returnToProc();
EXTERN void startNewProc();

/* this flag records whether we are in OS code or user code so that we  */
/* can avoid context switches in OS code.                               */
short inOsFlag;

/* this may be more appropriate in the time module, but the system clock*/
/* is an intimate companion with scheduling and therefore appears here. */
long osClock;                     /* counts clock ticks since startup   */


/* process scheduling, creation, etc... */

/* A note regarding ENTERINGOS and LEAVINGOS:                               */
/*  -  these routines protect against context switches during OS execution. */
/*  -  these routines are only called (with small exceptions) from interface*/
/*     (PUBLIC) routines. Calling LEAVINGOS from an OS routine called by    */
/*     some other OS routine could leave things in an unprotected state.    */
/*  -  routines which never get control back (call runNextProc) must be in  */
/*     OS context at the time of the call to runNextProc (runNextProc must  */
/*     be protected), but do not have to leave OS context as this will be   */
/*     done through runNextProc either by callNclean (for a new process) or */
/*     by SchedM (for a returning process).                                 */
/*  -  routines that call ShedM must be in OS context at the time of call,  */
/*     but don't need to leave OS context after call as this will be done   */
/*     by SchedM as soon as a process is resumed.                           */

/* this routine gets the next ready process and starts it running.          */
void runNextProc()
{
    if ( ! ISMTQ(RHIG) )  /* if the high priority process is ready to run   */
        DEQ(RHIG, currp)
    else if ( ! ISMTQ(RNOR) )        /* otherwise get normal prio process   */
        DEQ(RNOR, currp)
    else
        DEQ(RLOW, currp);

    /* set the process to run for a full quantum (if burst lasts that long  */
    SetQuantumRemaining( currp, GetQuantumLength( currp ) );

    if (GetNew( currp ))
        {
        /* make sure we know this is not new next time.                     */
        SetNew( currp, 0 );

        startNewProc(callNclean, GetSp( currp ));
        }
    else
        returnToProc( GetSavearea( currp ) );
}

/* provide a way for users to perform context switches                     */
/* do I want to save and restore the same process? */
PUBLIC void Sched()
{
    ENTERINGOS;

    SchedM;

    LEAVINGOS;
}

/* routines to manage the system clock and clock ticks.                    */

static void startClock( int useconds )
/* int useconds;    tick interval in microseconds                          */
{
    struct itimerval interval;

    /* clock will reset itself at each expiry to this interval             */
    interval.it_interval.tv_sec   = (useconds / 1000000);
    interval.it_interval.tv_usec  = (useconds % 1000000);

    /* this sets the initial clock interval                                */
    interval.it_value.tv_sec   = (useconds / 1000000);
    interval.it_value.tv_usec  = (useconds % 1000000);

    /* now start the timer so that it signals with a SIGVTALRM at every tick */
    setitimer( ITIMER_VIRTUAL, &interval, NULL );

    osClock = INITCLOCKVAL; /* osClock counts clock ticks since startup (now)*/

}

/* in a port to a multiprocessor, this routine could be split up into one */
/* part which is called (thorugh an interrupt) by each processor, and a   */
/* second part which is called (on interrupt) by only one processor.      */
static void tickHandler(int sig)
{
    register short wasInOs;
    register short quantumExpired;

    wasInOs = INOSCTXT;

    ENTERINGOS;

    /* keep track of time                                                 */
    UPDATECLOCK;

    /* see if the running processes quantum has expired                   */
    quantumExpired = (GetQuantumRemaining(currp)-- < 0);

    /* if there is a reason to do a context switch, and we were not       */
    /* running in the OS at the time of interrupt, do a context switch.   */
    if( ! wasInOs )
	{
	if( quantumExpired || (readySleepers() > 0) )
	    {
	    /* due to the unnatural return from this routine (i.e. SchedM),   */
	    /* we must turn the interrupts back on using sigsetmask().        */
     sigset_t mask;

  sigemptyset(&mask);
  /* sigaddset(&mask, SIGVTALRM | SIGIO); Changed Feb 8, 2002*/
  sigaddset(&mask, SIGVTALRM);
  sigaddset(&mask, SIGIO);
  if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
    perror("sigprocmask");
  }
 
	    SchedM;
	    }

	LEAVINGOS;
	}
}

/* the scheduling software requires that there is always one process ready */
/* to run. This process fulfills this requirement.                         */
PROCESS nullProcess( int useconds )
{
    int retVal;
    struct timeval timeout;

    while( 1 )
        {
        timeout.tv_sec   = (useconds / 1000000);
        timeout.tv_usec  = (useconds % 1000000);

        /* wait for a while, and then cause a clock tick                   */
        retVal = select( 0, 0, 0, 0, &timeout );
        if( retVal == 0 )
             kill( getpid(), SIGVTALRM );
        }

}


/* startos begins execution of the os by setting up the system clock and   */
/* loading the first process.                                              */
void startos()
{
  struct sigaction act;
  
  act.sa_handler = tickHandler;
  memset(&act.sa_mask,0,sizeof(sigset_t));
  act.sa_flags = 0;
  
  ENTERINGOS;
  
  /* register "tickHandler" routine to process system clock ticks.       */
  sigaction(SIGVTALRM, &act, NULL);	
  
  /* start the system clock running                                      */
  startClock( TICKINTERVAL );
  
  /* create the null process (always ready to run)                       */
  Create((void(*)())nullProcess, 16000, "nullProcess", 
	 (void *)TICKINTERVAL, LOW, -1);
  
  if( numbProcs )  /* if there are any user processes left */
    runNextProc();
  else    /* there are no more left to run - quit */
    {
      printf("\nexiting threads\n");
      exit(0);
    }
}


