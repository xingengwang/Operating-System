/*# Dwight Makaroff   */
/*# student number    */
/*# nsid              */
/*# November 7, 2008  */

/* No part of this program may be used for any profit or example for any
   purpose other than for help in A2 for CMPT 332 at the University
   of Saskatchewan in the fall term of 2012-13 without the expressed
   written consent of the author.

   *********** reader-writer.c ***********
*/

/*Yuchen Lin, yul761, 11138672
Xingeng Wang, xiw031, 11144515
*/


#include <stdio.h>


#include <standards.h>
#include <os.h>

#include <monitor.h>

#define OKtoWrite 1
#define OKtoRead  0

static int numReaders = 0;
static int busy =0;


void StartRead()
{
  MonEnter();

  /* if there is a writer */
  if (busy)
    {
      MonWait(OKtoRead);
    }
  /* once we are signaled, we can continue.
   * There will be at least one reader, so we can be assured
   * that no writers will get to go. No other readers will get
   * to go if we do not signal the rest of the readers, so each
   * reader that wakes up does a signal on OKtoRead. This will be OK
   * since the last signal will have no effect on a late-coming
   * reader.
   */
  numReaders++;
  printf("starting value of numReaders = %d\n", numReaders);
  
  MonSignal(OKtoRead);
  
  MonLeave();

}

void StopRead()
{
  MonEnter();
  numReaders--;
  printf("stopping value of numReaders = %d\n", numReaders);
  /* Only signal the writer when all readers have gone */
  if (numReaders == 0) MonSignal(OKtoWrite);
  MonLeave();
  
  
}

void StartWrite()
{
  MonEnter(); 

  /* if there are any readers or a writer */
  if ((numReaders !=0) || busy)
    MonWait(OKtoWrite);
  busy = 1;
  MonLeave();
}

void StopWrite()
{
  MonEnter();

  busy = 0;

  //printf("about to signal reader %d\n", MyPid());
  /* wake up any waiting readers */
  /* this gives priority to readers */
  /* could be a signal to the next writer, if there is one, but
   * then we'd need a mechanism to eventually signal a reader.
   * We couldn't signal a writer and then immediately signal a reader.
  */
  MonSignal(OKtoRead);

  MonLeave();

}
