#Yuchen Lin
#make file

UNAME_S=$(shell uname -s)

CC = gcc
CFLAGS = -Wall -pendantic

ifeq($(UNAME_S,Linux)
      Target = ptA1.exe
endif

all: $(Target)

partA1.exe: ptA1.o commonWin.o
	  $(CC) $(CFLAGS) -o ptA1.exe ptA1.o commonWin.o
	  
ptA1.o: ptA1.c
	  $(CC) $(CFLAGS) -c ptA1.o  ptA1.c
	  
commonWin.o: commonWin.c
	  $(CC) $(CFLAGS) commonWin.o commonWin.c
clean:
	  rm -f *.o $(Target)