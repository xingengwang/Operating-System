
/* initialize PCB from IPC point of view */
#define IPCPCBINIT( PCB ) {                           \
    SetWaitingFirst(PCB, -1);                         \
    SetWaitingLast(PCB, -1);                          \
    SetWaitingNext(PCB, -1);                          \
    SetWaitingPrev(PCB, -1);                          \
    SetPeer(PCB, -1);                                 \
    }

/* reset IPC fields for newly creates process (note that remaining pointer */
/* fields should already be clear).                                        */
#define SETUPIPC( PCB ) {SetPeer( PCB, -1 );}

EXTERN void ipcProcCleanup( PID );
