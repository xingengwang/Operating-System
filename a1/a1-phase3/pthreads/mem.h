/* memory frame for stack of temporary memory */

/* memory header for memory allocation */
typedef struct hdr
    {
    int        len;
    BYTE       spot;
    BYTE       inuse;
    short      magicNumb;
    int        nameIndex;
    struct hdr *next;
    } MemHdr;

/* temporary memory structures        */
typedef struct chunk
    {
    int          length;
    int          remaining;
    struct chunk *next;
    BYTE         *mem;
    } MemChunk;

typedef struct frm
    {
    char       *name;
    PID        onStack;
    MemChunk   *chunks;
    int        nextChunkSize;
    struct frm *next;
    } MemFrame;

/* memory initialization routine - to be called on threads startup          */
#define INITMEM {OrphFrameList = NULL;}

/* to be called for every new PCB                                           */
#define MEMPCBINIT(X) {SetMyFrame(X,NULL);}

EXTERN void memProcCleanup( PID ); /* to be called on process death */
