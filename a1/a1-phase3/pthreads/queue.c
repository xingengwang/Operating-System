#include "kernel.h"

/* the following are arrays of the process descriptor queues   */
/* eg. qbeg[RHIG] and qend[RHIG] are the front and back of     */
/* high priority process queue.                                */
/* The size of the arrays must agree with the number of queues */
PID qbeg[NUMQS];
PID qend[NUMQS];

PNODE *findPidOnQ (PSTATE q, PID pid)  /* find process pid in queue q */
{
    if( PIDINRANGE( pid ) &&
        GetInUse( getPcbFromPid(pid) ) &&
        ( GetQueue( getPcbFromPid(pid) ) == q ) )

        return( getPcbFromPid( pid ) );

    else
        return( NULL );
}

