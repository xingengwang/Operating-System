
/* sets initial values for the PCB when a new PCB is initialized. */
#define SYNCHPCBINIT( PCB ) {                           \
    SetSemaphPrev(PCB, -1);                             \
    SetSemaphNext(PCB, -1);                             \
    SetSemaphore(PCB, -1);                              \
    }

#define SEMAPHINRANGE(X) (((X) >= 0) && ((X) < NUMBSEMAPHORES) &&\
                         (semaphores[X].inUse))

EXTERN void initSynch();
EXTERN void synchProcCleanup( PID );

