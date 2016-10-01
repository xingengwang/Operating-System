#include "kernel.h"

/* these routintes manage the system time functions (other that time slicing) */


/* the following structure and variables are for the elapsed timer facility   */
typedef struct
    {
    short running;
    int   startedAt;
    int   prevETime;
    } ETimer;

static ETimer etimers[NUMBETIMERS];

/* the timing wheels - long and short duration                                */
static PID longWheel [SHORTLENGTH + 1];
static PID shortWheel[SHORTLENGTH + 1];

void initTime()
{
    int wheelPos;
    int eTimer;

    for( wheelPos = 0; wheelPos <= SHORTLENGTH; wheelPos++ )
        {
        longWheel[wheelPos] = -1;
        shortWheel[wheelPos] = -1;
        }

    for( eTimer = 0; eTimer < NUMBETIMERS; eTimer++ )
        {
        etimers[eTimer].running = 0;
        etimers[eTimer].prevETime = 0;
        }
}

PUBLIC long Time()    /* public access to system time in units of clock ticks */
{
    long retVal;

    ENTERINGOS;

    retVal = TIME;

    LEAVINGOS;

    return( retVal );
}

/* load caller into list of sleepers at position "wheelPos"                   */
static void addSleeper( PID sleeper, PID *wheelPos )
{
    PNODE *sleeperPcb;

    sleeperPcb = getPcbFromPid( sleeper );

    if( *wheelPos != -1 )
        {
        SetSleepNext( sleeperPcb, *wheelPos );
        SetSleepPrev( getPcbFromPid(*wheelPos), sleeper );
        }
    else    /* no one is currently timed to expire at same point in time      */
        SetSleepNext( sleeperPcb, -1 );

    SetSleepPrev( sleeperPcb, -1 );
    *wheelPos = sleeper;

    SetWheelPos( sleeperPcb, wheelPos );
}

/* take sleeper "pid" off of his waiting wheel position.                      */
static void dequeueSleeper( PID pid )
{
    PID   *wheelPos;
    PNODE *sleeper;
    PID   prev, next;

    sleeper  = getPcbFromPid( pid );
    wheelPos = GetWheelPos( sleeper );
    prev     = GetSleepPrev( sleeper );
    next     = GetSleepNext( sleeper );

    if( prev == -1 )
        *wheelPos = next;
    else
        SetSleepNext( getPcbFromPid( prev ), next );

    if( next != -1 )
        SetSleepPrev( getPcbFromPid( next ), prev );

    SetWheelPos( sleeper, NULL );
    SetSleepNext( sleeper, -1 );
    SetSleepPrev( sleeper, -1 );
}

/* clean up time-related details when a process exits or is killed.           */
void timeProcCleanup( PID pid )
{
    PNODE *pcb;

    pcb = getPcbFromPid( pid );

    if( GetState( pcb ) == SLEP )
        dequeueSleeper( pid );
}

/* returns 0 on success - if duration is too long, returns -1 immediately     */
PUBLIC int Sleep( int ticks ) /* suspend calling proc for "ticks" clock ticks */
{
    int retVal;
    int wheelVal;
    PID *wheelPos;

    ENTERINGOS;

    if( ticks < 0 )
        retVal = -1;

    else
    if( ticks == 0 )
        retVal = 0;

    else
    if( ticks <= LONGLENGTH )
        {
        retVal = 0;
	readySleepers();  /* catch up on delayed sleepers...        */

        if( ticks <= SHORTLENGTH )    /* a short duration timer */
            {
            wheelVal = SHORTPART( ticks + TIME );
            wheelPos = shortWheel + wheelVal;
            }

        else    /* set for long duration                        */
            {
            int expiryTime = (ticks + TIME);

            SetShortLength( currp, SHORTPART( expiryTime ) );
            wheelVal = LONGPART( expiryTime );
            wheelPos = longWheel + wheelVal;
            }

        addSleeper( GetPid( currp ), wheelPos );
        SetState( currp, SLEP );
        }

    else
        retVal = -1;  /* duration too long - fail.              */

    SchedM;

    LEAVINGOS;

    return( retVal );
}

/* Take a currently sleeping process and change its wakeup time to "ticks"    */
/* clock ticks from time of call.                                             */
/* returns 0 on success - if duration is too long, returns -1 immediately     */
PUBLIC int ReSleep( PID sleeper, int ticks )
{
    int retVal;
    int wheelVal;
    PID *wheelPos;
    int doCtxtSwitch = 0;

    ENTERINGOS;

    readySleepers();  /* catch up on delayed sleepers...        */

    if( (ticks < 0) || (ticks > LONGLENGTH) || 
        (findPidOnQ(SLEP, sleeper) == NULL ) )
        retVal = -1;

    else
    if( ticks == 0 )    /* want to wake up sleeper now          */
        {
        PNODE *sleeperPcb;

        retVal = 0;

        dequeueSleeper( sleeper );/* take sleeper off wheel list*/
        sleeperPcb = getPcbFromPid( sleeper );
        SetState( sleeperPcb, READYQ(sleeperPcb) );
        DEQPPTR( SLEP, sleeperPcb );
        ENQ( GetState( sleeperPcb ), sleeperPcb );

        doCtxtSwitch = 1;  /* perform context switch on exit.   */
        }
    
    else
    if( ticks <= LONGLENGTH )
        {
        /* move sleeper to the wheelPos that reflects its new wakeup time */

        retVal = 0;

        dequeueSleeper( sleeper );/* take sleeper off wheel list*/

        if( ticks <= SHORTLENGTH )    /* a short duration timer */
            {
            wheelVal = SHORTPART( ticks + TIME );
            wheelPos = shortWheel + wheelVal;
            }

        else    /* set for long duration                        */
            {
            int expiryTime = (ticks + TIME);

            SetShortLength( getPcbFromPid(sleeper), SHORTPART( expiryTime ) );
            wheelVal = LONGPART( expiryTime );
            wheelPos = longWheel + wheelVal;
            }

        addSleeper( sleeper, wheelPos );
        }

    else
        retVal = -1;    /* duration too long - fail.            */

    if( doCtxtSwitch )
        SchedM;

    LEAVINGOS;

    return( retVal );
}


/* returns number of sleepers awoken                                          */
int readySleepers()
{
    static int timeOfLastCall = INITCLOCKVAL;

    int retVal = 0;
    int currentTime = TIME;
    PID sleeper;
    int time;

    /* we may have not been called for a few clock ticks - catch up           */
    for( time = (timeOfLastCall + 1); time <= currentTime; time++ )
        {
        if( SHORTPART( time ) == 0 )
            {
            /* advance any sleeper to the short duration queue */
            while( (sleeper = longWheel[LONGPART( time )]) != -1 )
                {
                dequeueSleeper( sleeper );
                addSleeper( sleeper, 
                       shortWheel + GetShortLength(getPcbFromPid(sleeper)) );
                }
            }
    
        while( (sleeper = shortWheel[SHORTPART( time )]) != -1 )
            {
            PNODE *sleeperPcb;
    
            retVal++;
    
            dequeueSleeper( sleeper );
            sleeperPcb = getPcbFromPid( sleeper );
            SetState( sleeperPcb, READYQ(sleeperPcb) );
            DEQPPTR( SLEP, sleeperPcb );
            ENQ( GetState( sleeperPcb ), sleeperPcb );
            }
        }

    timeOfLastCall = currentTime;

    return( retVal );
}


/* the following are elapsed timer routines.                                 */

/* reset timer to initial state.                                             */
PUBLIC int ResetETimer( int timer)
{
    int retVal;

    ENTERINGOS;
    if( ! ETIMERINRANGE( timer ) )
        retVal = -1;

    else
        {
        retVal = 0;
        etimers[timer].prevETime = 0;
        etimers[timer].running = 0;
        }

    LEAVINGOS;

    return ( retVal );
}

/* start (or restart) a timer.                                               */
PUBLIC int StartETimer( int timer )
{
    int retVal;

    ENTERINGOS;

    if( ! ETIMERINRANGE( timer ) )
        retVal = -1;

    else
        {
        retVal = 0;
        etimers[timer].running = 1;
        etimers[timer].startedAt = TIME;
        }

    LEAVINGOS;

    return ( retVal );
}

/* get time elapsed running time since last reset without stoping timer      */
PUBLIC int GetETimer( int timer )
{
    int retVal;

    ENTERINGOS;

    if( ! ETIMERINRANGE( timer )  )
        retVal = -1;

    else
    if( etimers[timer].running == 0 )
        retVal = etimers[timer].prevETime;

    else
        retVal = (TIME - etimers[timer].startedAt) + etimers[timer].prevETime; 

    LEAVINGOS;

    return ( retVal );
}

/* stop timer, returning elapsed running time since last reset.              */
PUBLIC int StopETimer( int timer )
{
    int retVal;

    ENTERINGOS;

    if( (! ETIMERINRANGE( timer )) || (etimers[timer].running == 0) )
        retVal = -1;

    else
        {
        etimers[timer].running = 0;
        etimers[timer].prevETime += (TIME - etimers[timer].startedAt);
        retVal = etimers[timer].prevETime;
        }

    LEAVINGOS;

    return ( retVal );
}

