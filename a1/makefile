#Xingeng Wang, xiw031, 11144515
#Yuchen Lin  , yul761, 11138672
#make file

ARCH=$(shell uname -m)
UNAME_S=$(shell uname -s)
DIR= /www/docs/classes/332/t1/pthreads
LIBNAME=pthreads

#UBC
UBCC= -l$(DIR) -m32
UBCL= -L$(DIR)/lib/$(UNAME_S)$(ARCH) -l$(LIBNAME)
UBCI= -I$(DIR)

CC = gcc
CFLAGS = -Wall -pendantic -g

ifeq($(UNAME_S),Linux)
	Target = partA2,partA3,mylisttest
else
	Target = partA1.exe
endif

ifeq($(ARCH),x86_64)
	BITZ=64
else
	BITX=32
endif

all: $(Target)

partA1.exe: ptA1.o commonWin.o
	  $(CC) $(CFLAGS) -o partA1.exe partA1.o commonWin.os
	  
partA1.o: partA1.c
	$(CC) $(CFLAGS) -c partA1.c -I.
	
partA2: partA2.o 
	 $(CC) $(CFLAGS) -o partA2 partA1.o $(UBCL) $(UBCI) -m$(BITZ)
	 
partA2.o: partA2.c
	$(CC) $(CFLAGS) $(UBCL) -c partA1.c -I. $(UBCI) -m$(BITZ)

commonWin.o: commonWin.c
	  $(CC) $(CFLAGS) commonWin.o commonWin.c
clean:
	  rm -f *.o $(Target)
