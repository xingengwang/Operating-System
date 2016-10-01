#include "kernel.h"

/* inter-process communication primitives */

/* load caller into list of senders waiting to deliver message to "to"        */
static void addWaitingSender( PID to )
{
    PNODE *toPcb;

    toPcb = getPcbFromPid( to );
    
    if( GetWaitingFirst( toPcb ) != -1 )
        {
        SetWaitingPrev( currp, GetWaitingLast( toPcb ) );
        SetWaitingNext( getPcbFromPid(GetWaitingLast(toPcb)), GetPid(currp) );
        }
    else    /* no one is currently waiting to send a message to "to"          */
        {   
        SetWaitingFirst( toPcb, GetPid( currp ) );
        SetWaitingPrev( currp, -1 );
        }   

    SetWaitingNext( currp, -1 );
    SetWaitingLast( toPcb, GetPid( currp ) );
}

/* take sender "pid" off of his waiting list.                                 */
static void dequeueSender( PID pid )
{
    PID   prev, next;
    PNODE *thisPcb;

    thisPcb = getPcbFromPid( pid );
    prev    = GetWaitingPrev( thisPcb );
    next    = GetWaitingNext( thisPcb );

    if( prev == -1 )
        SetWaitingFirst( getPcbFromPid(GetPeer(thisPcb)), next );
    else
        SetWaitingNext( getPcbFromPid( prev ), next );

    if( next == -1 )
        SetWaitingLast( getPcbFromPid(GetPeer(thisPcb)), prev );
    else
        SetWaitingPrev( getPcbFromPid( next ), prev );

    SetWaitingNext( thisPcb, -1 );
    SetWaitingPrev( thisPcb, -1 );
}

/* to be called for every process leaving the system                          */
void ipcProcCleanup( PID pid )
{
    PID waitingProc;
    PNODE *waitingPcb;

    while( (waitingProc = GetWaitingFirst(getPcbFromPid(pid))) != -1 )
        {
        waitingPcb = getPcbFromPid( waitingProc );

        dequeueSender( waitingProc );
        SetRpl( waitingPcb, (char *)NOSUCHPROC );
        DEQPPTR( SBLK, waitingPcb );
        SetState( waitingPcb, READYQ(waitingPcb) );
        ENQ( GetState( waitingPcb ), waitingPcb );
        }
}


PUBLIC void *Send ( PID to, void *msg, int *len )
/* returns reply pointer */
{
    PNODE *pptr;

    ENTERINGOS;

    if ( (pptr = findPidOnQ( RBLK, to ) ))     /* if to is waiting for this msg */
        {
        SetMsg( pptr, msg );
        SetPeer( pptr, GetPid( currp ) );    /* record message source         */
        if( len )
            SetMsglen( pptr, *len )
        else
            SetMsglen( pptr, 0 );

        /* now this process must wait for a reply */
        SetState( currp, WTFR );
        SetState( pptr, READYQ(pptr) );
        DEQPPTR(RBLK, pptr);
        ENQ(GetState(pptr), pptr);
        }
    else    /* "to" is not waiting for a message right now    */
        {
        if(pExists(to))  /* make sure "to" is alive           */
            {
            addWaitingSender( to ); /* add to receivers waiting list.         */
            SetMsg( currp, msg );
            SetPeer( currp, to );

            if( len )
                SetMsglen( currp, *len )
            else
                SetMsglen( currp, 0 );

            SetState( currp, SBLK );
            }
        else
            SetRpl( currp, (char *) NOSUCHPROC );
        }

    SchedM;

    LEAVINGOS;

    /* return replied length */
    if( len )
        *len = GetMsglen( currp );

    return( GetRpl( currp ) );
}

PUBLIC int MsgWaits()     /* is a message waiting for me to read ?*/
{
    int retVal;

    ENTERINGOS;

    if( GetWaitingFirst( currp ) == -1 )
        retVal = 0;
    else
        retVal = 1;

    LEAVINGOS;

    return( retVal );
}

PUBLIC void *Receive( PID *pid, int *len )       /* returns message pointer */
{
    PID   sender;
    PNODE *senderPcb;
    short doCtxtsw = 0;

    ENTERINGOS;

    sender = GetWaitingFirst( currp );

    if( sender != -1 ) /* if there is anyone sending to me */
        {
        senderPcb = getPcbFromPid( sender );

        SetMsg( currp, GetMsg( senderPcb ) );
        SetPeer( currp, GetPid( senderPcb ) );
        SetMsglen( currp, GetMsglen( senderPcb ) );

        dequeueSender( sender );     /* take sender off waiting queue       */

        SetState( senderPcb, WTFR ); /* senders state now waiting for reply */
        DEQPPTR(SBLK, senderPcb);
        ENQ(GetState( senderPcb ), senderPcb);
        }
    else     /* no one currently wants to send to me, block */
        {
        SetState( currp, RBLK );
        doCtxtsw = 1;
        }

    if( doCtxtsw )
        SchedM;

    LEAVINGOS;    /* to reflect reality - should come right after SchedM    */

    *pid = GetPeer( currp );
    SetPeer( currp, -1 );

    if( len )
        *len = GetMsglen( currp );

    return(GetMsg( currp ));
}

/* returns 0 if ok, else NOSUCHPROC */
PUBLIC int Reply(PID sender, void *msg, int len) 
{
    int   result;
    PNODE *senderPcb;

    ENTERINGOS;

    if((senderPcb = findPidOnQ(WTFR, sender))) /* pid is waiting for my reply */
        {
        SetRpl( senderPcb, msg );
        SetMsglen( senderPcb, len );
        result = 0;
        SetState( senderPcb, READYQ( senderPcb ) );
        SetPeer( senderPcb, -1 );
        DEQPPTR(WTFR, senderPcb);
        ENQ(GetState( senderPcb ), senderPcb);
        }
    else
        result = NOSUCHPROC;

    SchedM;

    LEAVINGOS;

    return(result);
}

