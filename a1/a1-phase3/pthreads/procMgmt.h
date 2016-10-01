/****************************************************************************/

/* PIDs consist of a machine portion (high 12 bytes) and a local */
/* id portion (low 20 bytes). This all assumes that an int is 32 */
/* bits wide.                                                    */

/* get the local id portion of the pid                           */
#define LID(X) ((X) & 0x000fffff)

/* get the machine id portion of the pid                         */
#define MID(X) (((X) & 0xfff00000) >> 20)

/* make a complete pid out of a machine id and a local id        */
#define MAKEPID(M, L) (((L) & 0x000fffff) | (((M) & 0xfff) << 20))

/* does the pid fall within the valid range?                     */
#define PIDINRANGE(X) (((LID(X)) >= 0) && (LID(X) < currProcTabSize))

/* get the process control block given the PID (NULL if no such process) */
#define getPcbFromPid(X)   \
            ((PIDINRANGE(X))? (procTable + (LID(X))): NULL)

/* does process with PID X exist? */
#define pExists(X) ( PIDINRANGE(X) && GetInUse(getPcbFromPid(X)) )


#ifdef hp700
#define STACKBOT(X) GetStmembeg(X)
#else
#define STACKBOT(X) GetStkend(X)
#endif


/* this macro sets up PCB fields associated with process management     */
/* every time a process is created.                                     */
#define SETUPPROCMGMT( PD, PRIO, STKSIZE, ADDR, ARG )                   \
    {                                                                   \
    SetPrio( PD, PRIO );                                                \
    SetState( PD, READYQ(PD) );                                         \
    SetStkend( PD, GetStmembeg( PD ) + (STKSIZE >> 2) );                \
    SetSp( PD, STACKBOT(PD) );                                          \
    SetProc_addr( PD, ADDR );                                           \
    SetProc_arg( PD, ARG );                                             \
    }


/* process table related info                 */
EXTERN short currProcTabSize;  /* current size of the process table   */
EXTERN PNODE *procTable;       /* starting address of the proc table  */

/* system processes */
#define NUMSYSPROCS 1          /* number of system processes (nullProcess) */

EXTERN PNODE *currp;           /* process currently running           */

EXTERN int numbProcs;          /* number of existing processes        */

EXTERN void procIsLeaving( PID );  /* called every time a process leaves */
EXTERN void callNclean();          /* entry point for all new processes  */
EXTERN void makeInitialPcbs();     /* sets up PCBs at startup            */

