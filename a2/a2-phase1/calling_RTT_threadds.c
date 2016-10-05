#include <stdlib.h>
#include <stdio.h>
#include ss<list.h>
#include "include/rtthreads.h"
#include "include/RttCommon.h"

static RttThreadId threadWaitInput;
static RtSem sem;

RTTTHREAD WaitForInput(void*);


int mainp(int argc, char *argv[])
{
  RttSchAttr input_attrs;
  RttAllocSem(&sem, 1, RTTFCFS);
  
  RttCreate(&threadWaitInput, WaitForInput, 160000, "input", (void*)1, input_attrs, RTTUSR);
  return 0;
}

RTTTHREAD WaitForInput(void*)
{
  char* buffer;
  buffer =RttMalloc(BUFSIZE * sizeof(char));
  RttFree(buffer);
  RttUSleep(10000);
  
}