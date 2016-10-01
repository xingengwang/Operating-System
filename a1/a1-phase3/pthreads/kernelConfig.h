/* configuration constants */


#define MAXPNAMELEN 17        /* max process name length */

#define INITPROCTABSIZE 1      /* initial size of process table       */
#define PROCTABSIZEINCR 1      /* size of each procTable growth       */

/* clock tick interval in microseconds                 */
#define TICKINTERVAL   50000

/* initial value of clock (usually zero)               */
#define INITCLOCKVAL   0

/* default quantum length in clock ticks.              */
/* #define DEFAULTQUANTUM 4 */
#define DEFAULTQUANTUM 10

/* the number of elapsed timers in the system          */
/* Request to increase this number made November 2005.
 * Temporary agreement to do so. D. Makaroff 11/14/05
 */
/*#define NUMBETIMERS 10 */

#define NUMBETIMERS 50


/* the number of semaphores available from the system  */
#define NUMBSEMAPHORES 10

/* defines for the memory allocation system            */
#define WDBNDRY 4    /* memory alignment               */

#define FIRSTCHUNKSIZE 4 /* first chunk size given as entry into table below */
#define LASTCHUNKSIZE 12 /* last chunk size given as entry into table below  */
