/* standards.h file for UBC pthreads */
/* Copyright UBC 1989 */

#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define PROCESS void
#define PUBLIC
#define PRIV static
#define EXTERN extern

#define BYTE unsigned char

#define MAXIMUM(X,Y) (((X) < (Y))?(Y):(X))
#define MINIMUM(X,Y) (((X) < (Y))?(X):(Y))
