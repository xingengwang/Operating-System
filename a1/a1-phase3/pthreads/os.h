
#include "standards.h"

/* for error return values */
EXTERN int errno;

typedef int  PID;             /* process id type       */
typedef char PPRIO;           /* process priority type */

/* snd - rcv - rpl (send, receive, reply) error codes  */
#define NOSUCHPROC -1  /* no such destination process  */

/* process priorities and creation constants    */
#define HIGH ((PPRIO) 0 ) /*  high priority     */
#define NORM ((PPRIO) 1 ) /*  normal priority   */
#define LOW  ((PPRIO) 2 ) /*  low priority      */

#define USR  1 /* a user process */
#define SYS  0 /* a system or server process (perpetual) */

#define PNUL  (-1)         /* invalid process identifier */

EXTERN void *Send ( PID, void *, int * );
EXTERN int  MsgWaits();
EXTERN void *Receive( PID *, int * );
EXTERN int  Reply( PID, void *, int );
EXTERN PID  MyPid();
EXTERN PID  NameToPid ( char * );
EXTERN int  PExists( PID );
EXTERN void Suspend();
EXTERN int  Resume( PID );
EXTERN PID  Kill ( PID );
EXTERN void Pexit();
EXTERN PID  Create( void(*)(),  int, char *, void *, PPRIO, int );
EXTERN void Sched();
EXTERN long Time();
EXTERN int Sleep( int );
EXTERN int ReSleep( PID, int );
EXTERN int ResetETimer( int );
EXTERN int StartETimer( int );
EXTERN int GetETimer( int );
EXTERN int StopETimer( int );
EXTERN int P( int );
EXTERN int V( int );
EXTERN int NewSem(int );
EXTERN void FreeSem(int);
EXTERN int InitSemaph( int, int );
EXTERN BYTE *Malloc( int );
EXTERN void Free( void *);
EXTERN BYTE *Realloc( void *, int );
EXTERN void NewFrame();
EXTERN void FreeFrame();
EXTERN void PushFrame( void * );
EXTERN void *PopFrame();
EXTERN void SwapFrame();
EXTERN int  SaveFrameOnStack( void **, void * );
EXTERN void *PopFrameFromStack( void ** );
EXTERN void ClearFrameStack( void ** );
EXTERN BYTE *TempFrameMalloc( void *, int );
EXTERN BYTE *TempMalloc( int );
EXTERN void FreeTempMem();
EXTERN int TransferTempMem( PID );
EXTERN int TransferTempMem( PID topid );
EXTERN void ShowMalloc();
EXTERN BYTE *NamedMalloc( char *name, int size );
EXTERN int FrameMemUse( void *frm, int *allocated );
EXTERN void NewNamedFrame( char *name );
EXTERN void ShowMem();
