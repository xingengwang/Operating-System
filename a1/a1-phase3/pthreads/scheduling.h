
EXTERN short inOsFlag;

/* these macros record whether we are in OS code or user code so that we*/
/* can avoid context switches in OS code.                               */

#define INOSCTXT    inOsFlag
#define ENTERINGOS  inOsFlag = 1
#define LEAVINGOS   inOsFlag = 0

/* this may be more appropriate in the time module, but the system clock*/
/* is an intimate companion with scheduling and therefore appears here. */
/* os time in clock ticks since system startup */
#define TIME (osClock)
#define UPDATECLOCK (osClock++)
EXTERN long osClock;              /* counts clock ticks since startup   */


/* this is the macro used to perform a context switch. */
/* Note that a ready process regains control of the CPU by re-emerging */
/* (returning) from saveProcContext() with a return value of 0. If the */
/* return value from saveProcContext() is not 0, this is just a process*/
/* returning from saving its state.                                    */

#define SchedM {                                                         \
                   if( saveProcContext(GetSavearea( currp )) )           \
                       {                                                 \
                       ENQ(GetState( currp ), currp);                    \
                       runNextProc();                                    \
                       }                                                 \
                   else                                                  \
                       LEAVINGOS; /* back in user proc */                \
               }

/* sets up scheduling information for a new process */
#define SETUPSCHED(X, Q) {                       \
         SetQuantumLength( X, Q );               \
         SetNew( X, 1 );                         \
         }

#define SHEDULINGINIT( pcb )  /* for now, nothing to do here */

EXTERN void startos();
EXTERN void runNextProc();
EXTERN int * saveProcContext(int *Save);
