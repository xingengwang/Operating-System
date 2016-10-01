#include "kernel.h"

EXTERN void mainp();

/****************************************************************************/
/* This is a striped-to-the-bone Threads kernel. It is going to be developed*/
/* as a simulation environment for multiprocessor threads applications. It  */
/* is also designed to be more efficient than the last version of threads,  */
/* and therefore should be the starting point for any port to a bare machine*/
/****************************************************************************/
void initos()
/* does initializing stuff - to be called before any creates */
{
    currp = (PNODE *) 0;

    /* initializes OS queues               */
    INITQ;

    /* initializes time data structures.   */
    initTime();

    /* initializes the synchronization data structures */
    initSynch();

    /* create the initial table of process descriptors */
    makeInitialPcbs();
}


int main(int argc, char **argv)
{
    initos();
    mainp(argc, argv);
    startos();
    return (0);
    
}

