/*Yuchen Lin, yul761, 11138672
Xingeng Wang, xiw031, 11144515
*/
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
/*
#include "include/rtthreads.h"
#include "include/RttCommon.h"
*/
int MonInit();
int MonEnter();
int MonLeave();
int MonWait(int thread_indicator);
int MonSignal(int thread_indicator);
