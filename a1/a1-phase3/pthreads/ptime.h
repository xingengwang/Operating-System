/* these defines manage the system time functions (other that time slicing)  */

/* might want to make this configuration constants */
#define SHORTPART(X) ((X) & 0xff)           /* get short part of a time value*/
#define LONGPART(X)  (((X) & 0xff00) >> 8)  /* get long part of a time value */
#define SHORTLENGTH (0xff)                  /* longest time for a short timer*/
#define LONGLENGTH  (0xffff)                /* longest time for a long timer */

EXTERN int readySleepers();            /* returns number of woken sleepers   */
EXTERN void timeProcCleanup( PID );    /* clean up exiting sleeping process  */

EXTERN void initTime();                /* init time data structs on startup  */

#define TIMEPCBINIT( PCB ) {                           \
    SetSleepPrev(PCB, -1);                             \
    SetSleepNext(PCB, -1);                             \
    }

#define ETIMERINRANGE(X) (((X) >= 0) && ((X) < NUMBETIMERS))
