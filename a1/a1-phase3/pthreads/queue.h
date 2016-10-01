/* these are the process queues and states */
#define RHIG ((PSTATE) 0 ) /* high priority ready q                */
#define RNOR ((PSTATE) 1 ) /* normal priority ready q              */
#define RLOW ((PSTATE) 2 ) /* low priority ready q                 */
#define RBLK ((PSTATE) 3 ) /* q of receive blocked processes       */
#define SBLK ((PSTATE) 4 ) /* q of send blocked processes          */
#define WTFR ((PSTATE) 5 ) /* q of processes waiting for replys    */
#define SUSP ((PSTATE) 6 ) /* q of suspended processes             */
#define SLEP ((PSTATE) 7 ) /* q of sleeping processes              */
#define SEMB ((PSTATE) 8 ) /* q of semaphore blocked processes     */

#define NOQ  ((PSTATE) 0xff ) /* reserved for processes not on any queue  */

/* what follows are PCB queue routines and their related defines.           */
#define NUMQS (PSTATE)9
#define FIRST(Q)  (qbeg[Q])                       /* first item in queue    */
#define NEXT(P)   (GetNext(getPcbFromPid(P)) )    /* next item in queue     */
#define PREV(P)   (GetPrev(getPcbFromPid(P)) )    /* previous item in queue */
#define LAST(Q)   (qend[Q])                       /* last item in queue     */

/* returns a processes ready queue */
#define READYQ(X) ((GetPrio(X) == HIGH)? RHIG:((GetPrio(X) == NORM)? RNOR:RLOW))

#define INITQ  {  int MACROcount;                                        \
                  for (MACROcount = 0; MACROcount < NUMQS; MACROcount++) \
                     qbeg[MACROcount] = qend[MACROcount] = PNUL;         \
               }

/* is the Q empty */
#define ISMTQ(Q)  (qbeg[Q] == PNUL)

/* Enqueue an item. Insert in order of waketime if inserting into the sleep */
/* queue, else, put at end of queue                                         */
#define ENQ(Q,X) {                                                \
                 SetQueue((X), (Q));                              \
                 if(ISMTQ(Q)) {                                   \
                       qbeg[Q] = qend[Q] = (GetPid(X));           \
                       SetPrev( (X), PNUL );                      \
                       SetNext( (X), PNUL );                      \
                      }                                           \
                 else {                                           \
                       SetPrev((X), qend[Q]);                     \
                       SetNext(getPcbFromPid(qend[Q]), GetPid(X));\
                       qend[Q] = GetPid(X);                       \
                       SetNext( getPcbFromPid(qend[Q]), PNUL );   \
                      }                                           \
                 }

/* dequeue an item from the front of the queue */
#define DEQ(Q,X) {                                            \
                 (X) = getPcbFromPid(qbeg[Q]);                \
                 qbeg[Q] = GetNext( getPcbFromPid(qbeg[Q]) ); \
                 if (qbeg[Q] == PNUL)                         \
                     qend[Q] = PNUL;                          \
                 else                                         \
                     SetPrev( getPcbFromPid(qbeg[Q]), PNUL ); \
                 SetQueue( (X), NOQ );                        \
                 }

/* dequeue item pointed to by X in queue Q */
#define DEQPPTR(Q,X)                                          \
               {                                              \
               if (GetNext( (X) ) != PNUL)                    \
                   SetPrev( getPcbFromPid(GetNext( (X) )),    \
                            GetPrev( (X) ) );                 \
               else                                           \
                   qend[Q] = GetPrev( (X) );                  \
               if (GetPrev( (X) ) != PNUL)                    \
                   SetNext( getPcbFromPid(GetPrev( (X) )),    \
                            GetNext( (X) ) );                 \
               else                                           \
                   qbeg[Q] = GetNext( (X) );                  \
               SetQueue( (X), NOQ );                          \
               }

/* this macro sets up PCB fields associated with queue management       */
/* every time a new PCB is allocated.                                   */
#define QMGMTPCBINIT( pcb )                                             \
    {                                                                   \
    SetPrev( pcb, -1 );                                                 \
    SetNext( pcb, -1 );                                                 \
    }

EXTERN PID qbeg[NUMQS];        /* process queues (ready, blocked, ...) */
EXTERN PID qend[NUMQS];

EXTERN PNODE *findPidOnQ( PSTATE, PID );  /* returns PCB of pid if on Q */

