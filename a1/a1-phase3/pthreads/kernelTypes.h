/* this file contains types which are required by more than one kernel  */
/* module, but are not required by user code.                           */

typedef unsigned char PSTATE;          /* process state type    */

/* PCB fields required for IPC management.                              */
typedef struct
    {
    char *msg;                /* pointer to buffer of sent/awaited mesg */
    char *rpl;                /* pointer to reply buffer of sender      */
    PID  peer;                /* sender / receiver of message           */
    int  msglen;              /* length of message                      */

    PID waitingFirst;         /* list of procs sending to me            */
    PID waitingLast;          /* last proc in above list                */
    PID waitingNext;          /* next proc waiting for one I'm waiting on */
    PID waitingPrev;          /* prev proc waiting for one I'm waiting on */
    } IPCInfo;
#define GetMsg(X)          ((X)->ipcInfo.msg)
#define GetRpl(X)          ((X)->ipcInfo.rpl)
#define GetPeer(X)         ((X)->ipcInfo.peer)
#define GetMsglen(X)       ((X)->ipcInfo.msglen)
#define GetWaitingFirst(X) ((X)->ipcInfo.waitingFirst)
#define GetWaitingLast(X)  ((X)->ipcInfo.waitingLast)
#define GetWaitingNext(X)  ((X)->ipcInfo.waitingNext)
#define GetWaitingPrev(X)  ((X)->ipcInfo.waitingPrev)
#define SetMsg(X, V)         ((X)->ipcInfo.msg = (V));
#define SetRpl(X, V)         ((X)->ipcInfo.rpl = (V));
#define SetPeer(X, V)        ((X)->ipcInfo.peer = (V));
#define SetMsglen(X, V)      ((X)->ipcInfo.msglen = (V));
#define SetWaitingFirst(X,V) ((X)->ipcInfo.waitingFirst = (V))
#define SetWaitingLast(X,V)  ((X)->ipcInfo.waitingLast = (V))
#define SetWaitingNext(X,V)  ((X)->ipcInfo.waitingNext = (V))
#define SetWaitingPrev(X,V)  ((X)->ipcInfo.waitingPrev = (V))

/* PCB fields required for process management                           */
typedef struct
    {
    short inUse;              /* is there a process occupying this PCB? */

    PID pid;                  /* process id                             */
    PPRIO prio;               /* process priority = HIGH, NORM or LOW   */
    PSTATE state;             /* process state RHIG, RNOR, RBLK, etc.   */
    char pname[MAXPNAMELEN + 1]; /* process name as given by creator    */
    int *stmembeg;            /* stack limit (lowest stack byte)        */
    int *stkend;              /* current stack pointer                  */
    int *sp;                  /* current stack pointer                  */
    int stackSize;            /* size of processes stack.               */
    int level;                /* USR or SYS                             */

    void (*proc_addr)();      /* process entry point                    */
    void *proc_arg;           /* arg to process                         */
    } ProcMgmtInfo;
#define GetStmembeg(X)  ((X)->procMgmtInfo.stmembeg)
#define GetSp(X)        ((X)->procMgmtInfo.sp)
#define GetStkend(X)    ((X)->procMgmtInfo.stkend)
#define GetPid(X)       ((X)->procMgmtInfo.pid)
#define GetPrio(X)      ((X)->procMgmtInfo.prio)
#define GetState(X)     ((X)->procMgmtInfo.state)
#define GetPname(X)     ((X)->procMgmtInfo.pname)
#define GetProc_addr(X) ((X)->procMgmtInfo.proc_addr)
#define GetProc_arg(X)  ((X)->procMgmtInfo.proc_arg)
#define GetInUse(X)     ((X)->procMgmtInfo.inUse)
#define GetStackSize(X) ((X)->procMgmtInfo.stackSize)
#define GetLevel(X)     ((X)->procMgmtInfo.level)
#define SetStmembeg(X,V)  ((X)->procMgmtInfo.stmembeg = (V))
#define SetSp(X,V)        ((X)->procMgmtInfo.sp = (V))
#define SetStkend(X,V)    ((X)->procMgmtInfo.stkend = (V))
#define SetPid(X,V)       ((X)->procMgmtInfo.pid = (V))
#define SetPrio(X,V)      ((X)->procMgmtInfo.prio = (V))
#define SetState(X,V)     ((X)->procMgmtInfo.state = (V))
#define SetPname(X,V)     ((X)->procMgmtInfo.pname = (V))
#define SetProc_addr(X,V) ((X)->procMgmtInfo.proc_addr = (V))
#define SetProc_arg(X,V)  ((X)->procMgmtInfo.proc_arg = (V))
#define SetInUse(X,V)     ((X)->procMgmtInfo.inUse = (V))
#define SetStackSize(X,V) ((X)->procMgmtInfo.stackSize = (V))
#define SetLevel(X,V)     ((X)->procMgmtInfo.level = (V))


/* the following are for the queues that PCBs reside on                 */
typedef struct
    {
    PID prev;                 /* pointer to previous process descripter */
    PID next;                 /* pointer to next process descriptor     */
    PSTATE queue;             /* the queue this is process currently on?*/
    } QueueMgmtInfo;
#define GetPrev(X)      ((X)->queueMgmtInfo.prev)
#define GetNext(X)      ((X)->queueMgmtInfo.next)
#define GetQueue(X)     ((X)->queueMgmtInfo.queue)
#define SetPrev(X,V)      ((X)->queueMgmtInfo.prev = (V))
#define SetNext(X,V)      ((X)->queueMgmtInfo.next = (V))
#define SetQueue(X,V)     ((X)->queueMgmtInfo.queue = (V))

typedef struct
    {
    /* Need savearea of 11 for sparc, 13 for 68000, 22 for hp700        */
    int savearea[25];         /* context save area                      */

    short new;                /* if non-zero, process has not run yet   */

    short quantumLength;      /* length of quantum in clock ticks.      */
    short quantumRemaining;   /* remaining ticks for current quantum.   */
    } SchedInfo;
#define GetSavearea(X)         ((X)->schedInfo.savearea)
#define GetNew(X)              ((X)->schedInfo.new)
#define GetQuantumLength(X)    ((X)->schedInfo.quantumLength)
#define GetQuantumRemaining(X) ((X)->schedInfo.quantumRemaining)
#define SetSavearea(X,V)         ((X)->schedInfo.savearea = (V))
#define SetNew(X,V)              ((X)->schedInfo.new = (V))
#define SetQuantumLength(X,V)    ((X)->schedInfo.quantumLength = (V))
#define SetQuantumRemaining(X,V) ((X)->schedInfo.quantumRemaining = (V))

typedef struct
    {
    PID sleepNext;    /* next in list of sleeping processes             */
    PID sleepPrev;    /* previous in list of sleeping processes         */
    int shortLength;  /* remaining duration after long expiry           */
    PID *wheelPos;    /* head of sleep list this process is on          */
    } TimeInfo;
#define GetSleepNext(X)   ((X)->timeInfo.sleepNext)
#define GetSleepPrev(X)   ((X)->timeInfo.sleepPrev)
#define GetShortLength(X) ((X)->timeInfo.shortLength)
#define GetWheelPos(X)    ((X)->timeInfo.wheelPos)
#define SetSleepNext(X,V)   ((X)->timeInfo.sleepNext = (V))
#define SetSleepPrev(X,V)   ((X)->timeInfo.sleepPrev = (V))
#define SetShortLength(X,V) ((X)->timeInfo.shortLength = (V))
#define SetWheelPos(X,V)    ((X)->timeInfo.wheelPos = (V))

typedef struct
    {
    PID semaphNext;   /* next in list of procs blocked on this semaphore*/
    PID semaphPrev;   /* prev in list of procs blocked on this semaphore*/
    int semaphore;    /* semaphore this process is blocked on           */
    } SynchInfo;
#define GetSemaphNext(X)   ((X)->synchInfo.semaphNext)
#define GetSemaphPrev(X)   ((X)->synchInfo.semaphPrev)
#define GetSemaphore(X)    ((X)->synchInfo.semaphore)
#define SetSemaphNext(X,V)   ((X)->synchInfo.semaphNext = (V))
#define SetSemaphPrev(X,V)   ((X)->synchInfo.semaphPrev = (V))
#define SetSemaphore(X,V)    ((X)->synchInfo.semaphore = (V))

typedef struct
    {
    MemFrame *myFrame;
    } MemInfo;
#define GetMyFrame(X)      ((X)->memInfo.myFrame)
#define SetMyFrame(X,V)    ((X)->memInfo.myFrame = (V))



typedef struct                /* process descriptor */
{
    /* fields for general process management                            */
    ProcMgmtInfo  procMgmtInfo;

    /* fields for general queue management                              */
    QueueMgmtInfo queueMgmtInfo;

    /* fields required by the context switching code...                 */
    SchedInfo     schedInfo;

    /* the following for IPC */
    IPCInfo       ipcInfo;

    /* fields required by the time keeping routines.                    */
    TimeInfo      timeInfo;

    /* fields required by the process synchronization module            */
    SynchInfo     synchInfo;

    /* fields required for memory management                            */
    MemInfo       memInfo;

} PNODE;
