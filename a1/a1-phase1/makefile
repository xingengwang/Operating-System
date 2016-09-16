#Xingeng Wang, xiw031, 11144515
#Yuchen Lin  , yul761, 11138672
#make file

ARCH=$(shell uname -m)
UNAME_S=$(shell uname -s)
DIR= /student/cmpt332/pthreads
LIBNAME=pthreads

#UBC
UBCC= -l$(DIR) -m32
UBCL= -L$(DIR)/lib/$(UNAME_S)$(ARCH) -l$(LIBNAME)
UBCI= -I$(DIR)

CC = gcc
CFLAGS = -Wall -pedantic -g

ifeq ($(UNAME_S),Linux)
	Target = partA2 partA3 partA4
else
	Target = partA1.exe
endif

ifeq ($(ARCH),x86_64)
	BITZ=64
else
	BITX=32
endif

all: $(Target)

partA1.exe: partA1.o commonWin.o
	  $(CC) $(CFLAGS) -o partA1.exe partA1.o commonWin.o
	  
partA1.o: partA1.c
	$(CC) $(CFLAGS) -c partA1.c -I.
	
partA2: partA2.o commonWin.o
	 $(CC) $(CFLAGS) -o partA2 partA2.o commonWin.o $(UBCL) $(UBCI) -m$(BITZ)
	 
partA2.o: partA2.c
	$(CC) $(CFLAGS) $(UBCL) -c partA2.c -I. $(UBCI) -m$(BITZ)
	
partA3: partA3.o commonWin.o
	 $(CC) $(CFLAGS) -o partA3 partA3.o commonWin.o $(UBCL) $(UBCI) -m$(BITZ)
	 
partA3.o: partA3.c
	$(CC) $(CFLAGS) $(UBCL) -c partA3.c -I. $(UBCI) -m$(BITZ)

partA4: partA4.o commonWin.o
	 $(CC) $(CFLAGS) -o partA4 partA4.o commonWin.o

partA4.o: partA4.c
	$(CC) $(CFLAGS) -c partA4.c

commonWin.o: commonWin.c
	  $(CC) $(CFLAGS) -c commonWin.c
clean:
	  rm -f *.o $(Target)
