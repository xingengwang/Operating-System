/* UBC pthreads   mem.c
 */
#include <string.h>
#include "kernel.h"

/***************************************************************************/
/* many of these routines have private (within OS) and public entry points.*/
/* The public entry points could generally be changed to macros for        */
/* improved efficiency.                                                    */


/***************************************************************************/
/* headers and definitions                                                 */

/* function prototypes */
void unnotename( int i );
void getFrameFromOrphList( MemFrame *frame );
int saveFrameOnStack( MemFrame **list, MemFrame *frame );
void addFrameToOrphList( MemFrame *frame );

typedef struct mlist
    {
    int    allocs;    /* number of allocations for this memory length */
    int    frees;
    int    size;      /* length of memory chunks in this list         */
    int    moreMems;
    int    numbFree;  /* current number of free nodes on MemHdr list  */
    int    freeListMax;
    MemHdr *mem;      /* points to linked memory headers.             */
    } MemList;


#define MEMSIZES   13    /* number of entries in table below                 */

/* this structure describes memory allocation constants. Each entry describes*/
/* a list of free memory blocks of a certaion size. Temporary memory         */
/* allocation takes memory from and returns memory to this free list.        */
/* the format is as follows:                                                 */
/*   MBZ, MBZ, SIZE, MBZ, MBZ, MAX-FREES, MBN                                */
/* where, MBZ = must be zero, size = size of memory blocks on this list,     */
/* MAX-FREES = maximum size of free list, and MBN = must be null.            */

MemList MemArr[] = { { 0, 0, 16, 0, 0, 250, NULL },
                     { 0, 0, 32, 0, 0, 250, NULL },
                     { 0, 0, 48, 0, 0, 250, NULL },
                     { 0, 0, 100, 0, 0, 250, NULL },
                     { 0, 0, 400, 0, 0, 250, NULL },
                     { 0, 0, 1700, 0, 0, 250, NULL },
                     { 0, 0, 2100, 0, 0, 200, NULL },
                     { 0, 0, 4200, 0, 0, 200, NULL },
                     { 0, 0, 6100, 0, 0, 200, NULL },
                     { 0, 0, 9000, 0, 0, 150, NULL },
                     { 0, 0, 17000, 0, 0, 150, NULL },
                     { 0, 0, 21000, 0, 0, 100, NULL },
                     { 0, 0, 33000, 0, 0, 100, NULL }
                   };


/* allocate memory on boundries which are multiples of WDBNDRY */
#define WDBNDRYMASK (~(WDBNDRY - 1))

/* remember to recast when using word-boundrify (round up to word boundry) */
#define WDBNDRYFY(X)   ( (((int)(X)) + (WDBNDRY - 1)) & WDBNDRYMASK )

#define HDRSIZE   ( WDBNDRYFY (sizeof(MemHdr)))
#define MAGICNUMB ((short)0xdc43)    /* fence */

int    bytesused = 0;


/***************************************************************************/
/* OK - the debugging code is all above - now start the memory allocation  */
/* routines.                                                               */
/***************************************************************************/

/* get more memory from system */
MemHdr *moreMem( int size )
{
    MemHdr *temp;
    /*    MemHdr *next; */

    temp = (MemHdr *) malloc( size );

    if( ! temp )
        {
        printf("moreMem - can't get more memory from system\n");
        exit(90);
        }

    return( temp );
}



BYTE *MALLOC( int size )
{
    MemHdr *mem;

    if( size > MemArr[MEMSIZES - 1].size )
        {    /* really big - let UNIX handle this one */
        mem = moreMem( size + HDRSIZE );
        mem->next      = NULL;
        mem->len       = size;
        mem->magicNumb = MAGICNUMB;
        mem->spot      = (BYTE) -1;
        bytesused += (size + HDRSIZE);
        }

    else
        {
        int spot;
        /* first - find out which list we can take this from */
        
        spot = 0;
        while( size > MemArr[spot].size )
            spot++;

        /* have list (spot) from which to take memory - see if there is any */
        bytesused += ( MemArr[spot].size + HDRSIZE );
        MemArr[spot].allocs++;   /* update alloc count */

        if( MemArr[spot].mem )
            {
            mem = MemArr[spot].mem;
            MemArr[spot].mem = mem->next;
            MemArr[spot].numbFree--;  /* one less node on this free list */
            mem->next        = NULL;
            }
        else    /* no stored memory to take this from */
            {
            mem = moreMem( MemArr[spot].size + HDRSIZE );
            mem->next      = NULL;
            mem->len       = MemArr[spot].size;
            mem->spot      = (BYTE) spot;
            mem->magicNumb = MAGICNUMB;
	    MemArr[spot].moreMems++;
            }
        }
          
    mem->inuse = 1; /* tell FREE() it's ok to free this memory */
    mem->nameIndex = -1;
  
    return( (BYTE *) ((long)mem + HDRSIZE) );
}

/* public entry point to malloc */
PUBLIC BYTE *Malloc( int size )
{
    BYTE *retVal;

    ENTERINGOS;

    retVal = MALLOC( size );

    LEAVINGOS;

    return( retVal );
}

void FREE( MemHdr *mem )
{
    mem = (MemHdr*)((long)mem - HDRSIZE);    /* back up to header */
    if( mem->magicNumb != MAGICNUMB )
        {
        printf("Free - magic number messed up - can't free: ");
        return;
        }

    if( mem->next != NULL )    /* this was allocated by tempMalloc */
        return;

    if( mem->inuse )
        {
        unnotename( mem->nameIndex );

        bytesused -= ( mem->len + HDRSIZE );

        /* if mem is to big for list, or there are already enough  */
        /* of this size on list - then free to UNIX                */
        if(mem->spot == (BYTE) -1) 
            free( mem );

        else 
            {
            if (MemArr[mem->spot].numbFree >= MemArr[mem->spot].freeListMax) 
                free( mem );
    
            else
                {
                mem->next = MemArr[mem->spot].mem;
                MemArr[mem->spot].mem = mem;
                MemArr[mem->spot].numbFree++; /* one more node on free list */
	        mem->inuse = 0;
                }

            MemArr[mem->spot].frees++;
            }
        }

    else
        printf("Free - trying to FREE() already freed memory: ");
}

/* public entry point to FREE */
PUBLIC void Free( void *mem )
{
    ENTERINGOS;

    FREE( mem );

    LEAVINGOS;
}

/* this routine works only with Malloc'd (or NamedMalloc'd) mem. The results */
/* with tempMalloc'd mem will be unpredictable.                              */
BYTE *REALLOC( void *oldptr, int newlength )
{
    BYTE   *newmem;
    MemHdr *newhdr;
    MemHdr *oldhdr;
    int    copylen;
    int    nameIndex;
    void *memPtr;
    

    oldhdr = (MemHdr*)((long)oldptr - HDRSIZE);  /* back up to memory header */

    /* this magic number check catches messed up memory headers, and (if we */
    /* are luckey) catches most tempMalloc'd mem without memory faulting.   */
    if( oldhdr->magicNumb != MAGICNUMB )
        return( NULL );

    /* how much data is there to copy?                                      */
    copylen   = MINIMUM( newlength, oldhdr->len );

    /* allocate out new memory.                                             */
    newmem    = MALLOC( newlength );

    /* now copy the old data into the new memory location.                  */
    memPtr = memmove( newmem, oldptr, copylen );

    /* take the name for the new memory from the name of the old.           */
    newhdr    = (MemHdr*)((long)newmem - HDRSIZE);
    nameIndex = oldhdr->nameIndex;
    oldhdr->nameIndex = -1;    /* so stats are not messed up by FREE().     */
    newhdr->nameIndex = nameIndex;  

    FREE( oldptr );  /* toss oldmem */

    return( newmem );
}

/* public entry point to REALLOC */
PUBLIC BYTE *Realloc( void *oldptr, int newlength )
{
    BYTE *retVal;

    ENTERINGOS;

    retVal = REALLOC( oldptr, newlength );

    LEAVINGOS;

    return( retVal );
}



/****************************************************************************/
/* This is the code for the temporary memory facility. A process allocates  */
/* temporary memory using TempMalloc(). Each process has associated with it */
/* a stack of memory frames. The stack is empty on startup. A new frame will*/
/* be created when either a process calls TempMalloc and no frame exists,   */
/* or when NewFrame() is called. All temporary memory is allocated from the */
/* top memory frame. The memory in the memory stack can all be freed with   */
/* FreeTempMem(). FreeFrame() frees the top frame and its memory. The top   */
/* frame can be popped with PopFrame(), and can be pushed on any process'  */
/* stack with PushFrame(). When a process dies, all of the temporary memory */
/* on its stack goes away.                                                  */
/****************************************************************************/

/* returns the number of temporary memory frames for this process */
int numFrames()
{
    register MemFrame *temp;
    register int      count = 0;

    temp = GetMyFrame( currp );

    while( temp )
        {
        count++;
        temp = temp->next;
        }

    return( count );
}

/* add a memory chunk to this frame */
void addChunk( MemFrame *frame, int size )
{
    MemChunk *chunk;

    chunk = (MemChunk *) MALLOC( WDBNDRYFY( sizeof(MemChunk) ) + 
        WDBNDRYFY(size) );
    chunk->length     = WDBNDRYFY(size);
    chunk->remaining  = WDBNDRYFY(size);
    chunk->mem = (BYTE *) ( ((long) chunk) + WDBNDRYFY(sizeof(MemChunk)) );

    chunk->next     = frame->chunks;
    frame->chunks   = chunk;
}

/* put a new mem frame on top of calling process' memory stack */
void newFrame()
{
    MemFrame *frame;

    frame = (MemFrame *) MALLOC( sizeof( MemFrame ) );
    frame->chunks  = NULL;
    frame->onStack = GetPid( currp );
    frame->name    = NULL;
    frame->nextChunkSize = FIRSTCHUNKSIZE;

    frame->next    = GetMyFrame( currp );
    SetMyFrame( currp, frame );
}

/* public entry point to newFrame */
PUBLIC void NewFrame()
{
    ENTERINGOS;

    newFrame();

    LEAVINGOS;
}


/* push this frame onto memory stack */
void pushFrame( MemFrame *frame )
{
    if( frame )
        getFrameFromOrphList( frame );

   saveFrameOnStack( &(GetMyFrame( currp )), frame );
}

/* public entry point to PushFrame */
PUBLIC void PushFrame( void *frame )
{
    ENTERINGOS;

    pushFrame( frame );

    LEAVINGOS;
}

/* swaps the top two temporary memory stack frames */
void swapFrame()
{
    MemFrame *top, *next;

    top = GetMyFrame( currp );
    if( top )
        {
        next = top->next;
        if( next )
            {
            SetMyFrame( currp, next );
            top->next      = next->next;
            next->next     = top;
            }
        }
}

/* public entry point to swapFrame */
PUBLIC void SwapFrame()
{
    ENTERINGOS;

    swapFrame();

    LEAVINGOS;
}

int saveFrameOnStack( MemFrame **list, MemFrame *frame )
{
    int retval = 0;
    
    if( (frame != NULL) && (list != NULL) )
        {
        if( frame->onStack )
            {
            printf("saveFrameOnStack: frame is already on one stack\n");
            retval = -1;
            }
        else
            {
            frame->onStack = GetPid( currp );
            frame->next    = *list;
            *list          = frame;
            }
        }

    else
        retval = -1;

    return( retval );
}

/* public entry point to saveFrameOnStack */
PUBLIC int SaveFrameOnStack( void **list, void *frame )
{
    int retVal;

    ENTERINGOS;

    retVal = saveFrameOnStack( (MemFrame **)list, (MemFrame *)frame );

    LEAVINGOS;

    return( retVal );
}

void *popFrameFromStack( void **list )
{
    MemFrame *frame;

    frame = *((MemFrame **)list);

    if( frame )
        {
        frame->onStack = 0;
        *list          = frame->next;
        frame->next    = NULL;
        }

    return( (void *) frame );
}

/* public entry point to popFrameFromStac */
PUBLIC void *PopFrameFromStack( void **list )
{
    void *retVal;

    ENTERINGOS;

    retVal = popFrameFromStack( list );

    LEAVINGOS;

    return( retVal );
}



/* pop and return top memory frame */
void *popFrame()
{
    MemFrame *frame;

    frame = (MemFrame *) popFrameFromStack( (void **) &(GetMyFrame( currp )) );

    if( frame )
        addFrameToOrphList( frame );

    return( (void *) frame );
}

/* public entry point to popFrame */
PUBLIC void *PopFrame()
{
    void *retVal;

    ENTERINGOS;

    retVal = popFrame();

    LEAVINGOS;

    return( retVal );
}

/* free all tempmem on list of frames */
void clearFrameStack( MemFrame **list )
{
    MemFrame *frame, *nextFrame;
    MemChunk *chunk, *nextChunk;

    frame = *list;
    while( frame )
        {
        chunk = frame->chunks;
        while( chunk )
            {
            nextChunk = chunk->next;
            FREE( (void *) chunk );
            chunk = nextChunk;
            }

        getFrameFromOrphList( frame );

        nextFrame = frame->next;
        FREE( (void *) frame );
        frame = nextFrame;
        }

    *list = NULL;
}

/* public entry point to clearFrameStack */
PUBLIC void ClearFrameStack( void **list )
{
    ENTERINGOS;

    clearFrameStack( (MemFrame **)list );

    LEAVINGOS;
}



/* tempFrameMalloc and tempMalloc are the same except for the first few lines */
/* This is unfortunate but a macro for TempMalloc won't do.                   */
/* These routines allocate temporary (per-process) memory.                    */

/* allocate temporary memory and make it belong to the given memory frame.    */
BYTE *tempFrameMalloc( MemFrame *frame, int size )
{
    MemChunk *chunk;
    BYTE     *mem;

    if( size < 1 )
        return( NULL );

    /* check for the existence of a chunk and some memory...                */
    chunk = frame->chunks;
    if( (! chunk) || (chunk->remaining < (size+WDBNDRY)) )
        {
        addChunk( frame, MAXIMUM( MemArr[frame->nextChunkSize].size, size ) );

        /* adjust nextChunkSize so that if we have to later allocate another*/
        /* chunk - it will be bigger.                                       */
        if( frame->nextChunkSize < LASTCHUNKSIZE )
            frame->nextChunkSize++;

        chunk = frame->chunks;
        }
 
    /* if we are allocating more than 1 byte, put it on a proper boundry... */
    if( size != 1 )
        {
        /* adjust chunk->remaining to align to a proper boundry             */
        (chunk->remaining) &= WDBNDRYMASK;
        }

    mem = (BYTE *) (chunk->mem + chunk->length - chunk->remaining);

    chunk->remaining -= size;

    return( mem );
}


/* public entry point to tempFrameMalloc */
PUBLIC BYTE *TempFrameMalloc( void *frame, int size )
{
    BYTE *retVal;

    ENTERINGOS;

    retVal = tempFrameMalloc( (MemFrame *)frame, size );

    LEAVINGOS;

    return( retVal );
}

/* allocate memory from the top frame on this process' memory stack */
BYTE *tempMalloc( int size )
{
    MemChunk *chunk;
    BYTE     *mem;
    MemFrame *frame;

    if( size < 1 )
        return( NULL );

    /* if no frame, allocate one */
    if( GetMyFrame( currp )  == NULL )
        newFrame();

    frame = GetMyFrame( currp );

    /* check for the existence of a chunk and some memory...                */
    chunk = frame->chunks;
    if( (! chunk) || (chunk->remaining < (size+WDBNDRY)) )
        {
        addChunk( frame, MAXIMUM( MemArr[frame->nextChunkSize].size, size ) );

        /* adjust nextChunkSize so that if we have to later allocate another*/
        /* chunk - it will be bigger.                                       */
        if( frame->nextChunkSize < LASTCHUNKSIZE )
            frame->nextChunkSize++;

        chunk = frame->chunks;
        }

    /* if we are allocating more than 1 byte, put it on a proper boundry... */
    if( size != 1 )
        {
        /* adjust chunk->remaining to align to a proper boundry             */
        (chunk->remaining) &= WDBNDRYMASK;
        }

    mem = (BYTE *) (chunk->mem + chunk->length - chunk->remaining);

    chunk->remaining -= size;

    return( mem );
}

/* public entry point to tempMalloc */
PUBLIC BYTE *TempMalloc( int size )
{
    BYTE *retVal;

    ENTERINGOS;

    retVal = tempMalloc( size );

    LEAVINGOS;

    return( retVal );
}


/* free all tempmem of process proc */
void freeAllFrames( PNODE *proc )
{
    MemFrame *frame, *nextFrame;
    MemChunk *chunk, *nextChunk;

    frame = GetMyFrame( proc );
    while( frame )
        {
        chunk = frame->chunks;
        while( chunk )
            {
            nextChunk = chunk->next;
            FREE( (void *)chunk );
            chunk = nextChunk;
            }

        nextFrame = frame->next;
        FREE( (void *)frame );
        frame = nextFrame;
        }

    SetMyFrame( proc, NULL );
}

PUBLIC void FreeTempMem()
{
    ENTERINGOS;

    freeAllFrames( currp );

    LEAVINGOS;
}

/* free top frame and its memory */
void freeFrame() 
{
    MemFrame *frame;
    MemChunk *chunk, *next;

    frame = GetMyFrame( currp );

    if( frame )
        {
        SetMyFrame( currp, GetMyFrame(currp)->next );

        chunk = frame->chunks;
        while( chunk )
            {
            next = chunk->next;
            FREE( (void *)chunk );
            chunk = next;
            }
        FREE( (void *)frame );
        }
}


/* public entry point to freeFrame */
PUBLIC void FreeFrame()
{
    ENTERINGOS;

    freeFrame();

    LEAVINGOS;
}


/* transfers top memory frame from calling process to topid   */
/* returns 0 on success, -1 on failure                        */
PUBLIC int TransferTempMem( PID topid )
{
    PNODE    *destproc;
    MemFrame *frame;
    int      retVal;

    ENTERINGOS;

    if( (destproc = getPcbFromPid( topid )) )
        {
        frame = (MemFrame *) popFrame();
        if( frame )
            {
            getFrameFromOrphList( frame );
        
            saveFrameOnStack( &(GetMyFrame(destproc)), frame );
            }
        retVal = 0;
        }

    else
        retVal = -1;

    LEAVINGOS;

    return( retVal );
}

void memProcCleanup( PID pid )
{
    PNODE *pcb;

    pcb = getPcbFromPid( pid );

    freeAllFrames( pcb );
}


/***************************************************************************/
/* The next several hunderd lines of code are for memory tracing and       */
/* debugging. The interesting code is all above.                           */
/***************************************************************************/

typedef struct orphlist
    {
    MemFrame        *frame;
    struct orphlist *next;
    }    OrphList;

typedef struct frameinfo
    {
    char   *name;
    int    bytesused;
    int    chunksused;
    struct frameinfo *next;
    } FRAMEINFO;

typedef struct procmem
    {
    char      *name;
    PID       pid;
    FRAMEINFO *frames;
    int       numbframes;
    struct procmem *next;
    } PROCMEM;

OrphList *OrphFrameList;

typedef struct namedmalloc {
    char *name;
    int count;
    int amount;
} namedmalloc;

static namedmalloc *nameTab;
static int nameTabSize = 0;
static int nameTabNext = 0;

void unnotename( int i )
{
    if( (i < nameTabNext) && (i >= 0) )
        nameTab[i].count--;
}

int notename( char *name )
{
    int i;
    for( i = 0; i < nameTabNext; i++ )
        if( !strcmp( name, nameTab[i].name ) )
            break;

    if( i == nameTabNext )
        { /* Not found. */
        if (!nameTabSize)
            {
            nameTabSize = 100;
            nameTab = (namedmalloc *)
                MALLOC( sizeof(struct namedmalloc) * nameTabSize );
            }

        else if( nameTabNext == nameTabSize )
            {
            nameTabSize += 100;
            nameTab = (namedmalloc *)
                REALLOC(nameTab, sizeof(struct namedmalloc) * nameTabSize);
            }

        nameTab[i].name = name;
        nameTab[i].count = 0;
        nameTabNext++;
        }

    nameTab[i].count++;
    return(i);
}


PUBLIC void ShowMalloc()
{
    int i, len, size;
    int nmalloced, nunused;
    MemHdr *mem;

    ENTERINGOS;

    fprintf( stderr, "bytesused=%d\n", bytesused );
    fprintf( stderr, "MemArr:\n");
    nmalloced = nunused = 0;
    for( i = 0; i < MEMSIZES; i++ )
      {
	len = size = 0;
	for( mem = MemArr[i].mem; mem; mem = mem->next )
	  {
	    size += mem->len;
	    len++;
	  }
	fprintf( stderr,
         "%2d (%5d): allocs=%6d mallocs(%4d->%7d) frees=%6d unused(%3d->%7d)\n",
	 i, MemArr[i].size,
	 MemArr[i].allocs,
	 MemArr[i].moreMems, MemArr[i].moreMems * MemArr[i].size,
         MemArr[i].frees,
	 len, size );
	nmalloced += MemArr[i].moreMems * MemArr[i].size;
	nunused += size;
      }
    fprintf( stderr, "Totals: %d bytes malloced, %d bytes unused\n",
		nmalloced, nunused );

    fprintf( stderr, "\nNamed Mallocs:\n");
    for( i = 0; i < nameTabNext; i++ )
      {
	fprintf( stderr, "%20s: allocs=%4d\n",
		nameTab[i].name, nameTab[i].count );
      }

    LEAVINGOS;
}


BYTE *namedMalloc( char *name, int size )
{
    BYTE  *mem;
    MemHdr *hdr;
    int   index;

    mem = (BYTE *) MALLOC( size );

    index = notename( name );
    
    hdr = (MemHdr*)((long)mem - HDRSIZE);    /* back up to header */

    hdr->nameIndex = index;

    return( mem );
}

/* public entry point to namedMalloc */
PUBLIC BYTE *NamedMalloc( char *name, int size )
{
    BYTE *retVal;

    ENTERINGOS;

    retVal = namedMalloc( name, size );

    LEAVINGOS;

    return( retVal );
}


/* return allocation info for the given frame (defaults to the top frame) */
PUBLIC int FrameMemUse( void *frm, int *allocated )
{
    int mem, remaining;
    MemChunk *chunk;
    MemFrame *frame;

    ENTERINGOS;

    frame = (MemFrame *) frm;

    mem = remaining = 0;
    if( frame || (frame = GetMyFrame(currp)))
        {
        chunk = frame->chunks;
        while( chunk )
            {
            mem += chunk->length;
            remaining += chunk->remaining;
            chunk = chunk->next;
            }
        }
    if( allocated )
        *allocated = mem;

    LEAVINGOS;

    return( mem - remaining );
}


/* put a new mem frame on top of calling process' memory stack */
PUBLIC void NewNamedFrame( char *name )
{
    MemFrame *frame;

    ENTERINGOS;

    frame = (MemFrame *) namedMalloc( "frame", sizeof( MemFrame ) );

    frame->chunks  = NULL;
    frame->onStack = GetPid(currp);
    frame->name    = name;
    frame->nextChunkSize = FIRSTCHUNKSIZE;

    frame->next    = GetMyFrame(currp);
    SetMyFrame( currp, frame );

    LEAVINGOS;
}

/* the following are a bunch of debugging routines for the per-process memory */

void addFrameToOrphList( MemFrame *frame )
{
    OrphList *ol;
    OrphList *temp, *prev;

    temp = OrphFrameList;
    prev = NULL;
    while( temp && (temp->frame != frame) )
        {
        prev = temp;
        temp = temp->next;
        }

    if( temp )    /* Found what we are looking for?  Shouldn't happen! */
        printf("addFrameToOrphList: YIKES!!! HELP???\n");


    ol = (OrphList *) namedMalloc( "orphanlist", sizeof( OrphList ) );

    ol->frame     = frame;
    ol->next      = OrphFrameList;
    OrphFrameList = ol;
}

void getFrameFromOrphList( MemFrame *frame )
{
    OrphList *temp, *prev;

    prev = NULL;
    temp = OrphFrameList;

    while( temp && (temp->frame != frame) )
        {
        prev = temp;
        temp = temp->next;
        }

    if( temp )    /* found what we are looking for */
        {
        if( prev )
            prev->next = temp->next;
        else
            OrphFrameList = temp->next;

        FREE( (void *) temp );
        }
}



FRAMEINFO *frameMemInfo( MemFrame *frame )
{
    MemChunk  *chunk;
    FRAMEINFO *tempinfo;
    int chunks = 0, mem = 0;

    if( frame == NULL )
        return( NULL );

    chunk    = frame->chunks;
    tempinfo = (FRAMEINFO *) malloc( sizeof(FRAMEINFO) );

    tempinfo->name = frame->name;
    tempinfo->next = NULL;

    while( chunk )
        {
        chunks++;
        mem += (chunk->length - chunk->remaining);
        chunk = chunk->next;
        }

    tempinfo->bytesused  = mem;
    tempinfo->chunksused = chunks;

    return( tempinfo );
}

/* public entry point to frameMemInfo */
PUBLIC FRAMEINFO *FrameMemInfo( MemFrame *frame )
{
    FRAMEINFO *retVal;

    ENTERINGOS;

    retVal = frameMemInfo( frame );

    LEAVINGOS;

    return( retVal );
}


PROCMEM *procMemInfo( PID pid )
{
    PROCMEM *tempinfo;
    FRAMEINFO **frameinfo;
    MemFrame *frame;
    int frames = 0;
    PNODE *pcb;

    pcb = getPcbFromPid( pid );

    if( pcb == NULL )
        return( NULL );

    tempinfo  = (PROCMEM *) malloc( sizeof(PROCMEM) );
    frameinfo = &(tempinfo->frames);

    tempinfo->name = GetPname( pcb );
    tempinfo->pid  = GetPid( pcb );
    tempinfo->next = NULL;

    frame = GetMyFrame( pcb );

    while( frame )
        {
        frames++;

        *frameinfo = frameMemInfo( frame );
        frame      = frame->next;
        frameinfo  = &((*frameinfo)->next);
        }

     tempinfo->numbframes = frames;
     *frameinfo = NULL;

    return( tempinfo );
}

/* public entry point to procMemInfo */
PUBLIC PROCMEM *ProcMemInfo( PID pid )
{
    PROCMEM *retVal;

    ENTERINGOS;

    retVal = procMemInfo( pid );

    LEAVINGOS;

    return( retVal );
}

PROCMEM *orphMemInfo()
{
    PROCMEM *tempinfo;
    FRAMEINFO **frameinfo;
    OrphList *ol;
    MemFrame *frame;
    int frames = 0;

    tempinfo  = (PROCMEM *) malloc( sizeof(PROCMEM) );
    frameinfo = &(tempinfo->frames);

    tempinfo->name = "OrphanList";
    tempinfo->pid  = 0;
    tempinfo->next = NULL;

    ol = OrphFrameList;

    while( ol )
        {
        frames++;

        frame = ol->frame;

        *frameinfo = frameMemInfo( frame );
        ol         = ol->next;
        frameinfo  = &((*frameinfo)->next);
        }

     tempinfo->numbframes = frames;
     *frameinfo = NULL;

     return( tempinfo );
}


/* public entry point to orphMemInfo */
PUBLIC PROCMEM *OrphMemInfo()
{
    PROCMEM *retVal;

    ENTERINGOS;

    retVal = orphMemInfo();

    LEAVINGOS;

    return( retVal );
}


PROCMEM *allMemInfo()
{
    PROCMEM *headinfo, *tempinfo;
    PSTATE  state;
    PID     temp;

    headinfo = NULL;

    for( state = (PSTATE)0 ; state < (NUMQS - 1); state++)
        {
        for( temp = FIRST( state ); temp != PNUL; temp = NEXT( temp ) )
            {
            tempinfo = procMemInfo( temp );
            
            tempinfo->next = headinfo;
            headinfo       = tempinfo;
            }
        }

    /* get meminfo for the currently running process */
    tempinfo = procMemInfo( GetPid( currp ) );

    tempinfo->next = headinfo;
    headinfo       = tempinfo;

    /* get meminfo for the orphan frames */
    tempinfo = orphMemInfo();

    tempinfo->next = headinfo;
    headinfo       = tempinfo;

    return( headinfo );
}

/* public entry point to allMemInfo */
PUBLIC PROCMEM *AllMemInfo() 
{
    PROCMEM *retVal;

    ENTERINGOS;

    retVal = allMemInfo();

    LEAVINGOS;

    return( retVal );
}

PUBLIC void freeFrameMemList( FRAMEINFO *frameMemList )
{
    FRAMEINFO *curr, *next;

    ENTERINGOS;

    curr = frameMemList;

    while( curr )
        {
        next = curr->next;
        free( curr );
        curr = next;
        }

    LEAVINGOS;
}

void freeProcMemList( PROCMEM *procMemList )
{
    PROCMEM *curr, *next;

    curr = procMemList;

    while( curr )
        {
        next = curr->next;
        free( curr );
        curr = next;
        }
}

/*
 * N.B.: This function should not be called interactively from a debugger
 * if the program has been interrupted while in a Unix system call.
 * It appears as if the floating point calculation in this routine causes
 * something bad to happen when the program is resumed.  This may simply be
 * because of a bug in gdb 3.5.
 */

PUBLIC void ShowMem()
{
    int i, total;
    PROCMEM *pm, *pmhead;
    FRAMEINFO *f;
    MemList *m;
    int tot_frames, tot_bytes, tot_chunks;

    ENTERINGOS;

    tot_frames = 0;
    tot_bytes  = 0;
    tot_chunks = 0;

    fprintf(stderr, "numbProcs = %d\n", numbProcs);
    fprintf(stderr, "bytesused = %d\n", bytesused);
    fprintf(stderr, "numFrames = %d\n", numFrames());
    total = 0;

    fprintf(stderr, "Malloc Size Distribution:\n");
    for (i = 0, m = &MemArr[0]; i < MEMSIZES; i++, m++)
        total += m->allocs;
    for (i = 0, m = &MemArr[0]; i < MEMSIZES; i++, m++)
        fprintf(stderr, "Size<=%7d Allocs=%7d (%4.1f%%)\n",
                m->size, m->allocs,
                100.0 * (double) m->allocs / (double) total);

    pmhead = allMemInfo();

    fprintf(stderr, "\nMemory Frame Usage:\n");
    for (pm = pmhead; pm != NULL; pm = pm->next) {
	int bytesused, chunksused;

	bytesused  = 0;
	chunksused = 0;
        for (i = 0, f = pm->frames; f != NULL; i++, f = f->next) {
	    bytesused  += f->bytesused;
	    chunksused += f->chunksused;
	}

        fprintf(stderr, 
	    "%s (%d frames, %d bytes, %d chunks):\n", 
	    pm->name, i, bytesused, chunksused );

	tot_frames += i;
	tot_bytes  += bytesused;
	tot_chunks += chunksused;

        for (i = 0, f = pm->frames; f != NULL; i++, f = f->next) {
            fprintf(stderr, "%3d. %s: bytesused = %7d, chunksused = %5d\n",
                    i + 1, f->name, f->bytesused, f->chunksused);
        }
    }

    fprintf(stderr, 
	"\ntotal frames = %d, bytesused = %d, chunksused = %d\n", 
	tot_frames, tot_bytes, tot_chunks );

    freeProcMemList(pmhead);

    LEAVINGOS;
}

