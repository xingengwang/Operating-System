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
P= -L$(DIR)/lib/$(UNAME_S)$(ARCH) -l$(LIBNAME) -pthread
UBCI= -I$(DIR)


#RTT
RTTLC = -L/student/cmpt332/t1/rtt/lib/$(UNAME_S)$(ARCH) -m32 -lRtt
RTTLCUTILS = -L/student/cmpt332/t1/rtt/lib/$(UNAME_S)$(ARCH) -m32 -lRttUtils
RTTIC = -L/student/cmpt332/t1/rtt
#LOcal liaray dir
LOCAL_LIB = ./lib/$(UNAME_S)$(ARCH)
OBJ_LIB = ./obj/$(UNAME_S)$(ARCH)
LISTLIB = -L$(LOCAL_LIB)/ -llist
LISTLIB32 = -L$(LOCAL_LIB)/ -llist32
HONLIB = -L$(LOCAL_LIB)/ -lmonitor


.PHONY: all clean

#build
builddirs:
	mkdir -p $(LOCAL_LIB)/
	mkdir -p $(OBJ_LIB)/

CC = gcc
CFLAGS = -Wall -pedantic -g

ifeq ($(UNAME_S),Linux)
	Target = reader-writer-test s-chatL
else
	Target = s-chatS
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
	
partA2: partA2.o common.o
	 $(CC) $(CFLAGS) -o partA2 partA2.o common.o $(UBCL) $(UBCI) -m$(BITZ)
	 
partA2.o: partA2.c
	$(CC) $(CFLAGS) $(UBCL) -c partA2.c -I. $(UBCI) -m$(BITZ)
	
partA3: partA3.o common.o
	 $(CC) $(CFLAGS) -o partA3 partA3.o $(P) $(UBCI) -m$(BITZ)
	 
partA3.o: partA3.c
	$(CC) $(CFLAGS) $(UBCL) -c partA3.c -I. $(UBCI) -m$(BITZ)

partA4: partA4.o common.o
	 $(CC) $(CFLAGS) -o partA4 partA4.o common.o

partA4.o: partA4.c
	$(CC) $(CFLAGS) -c partA4.c

commonWin.o: commonWin.c
	  $(CC) $(CFLAGS) -c commonWin.c

common.o: common.c
	  $(CC) $(CFLAGS) -c common.c
	  
mytestlist:list_removers.o mytestlist.o list_adders.o list_movers.o
	$(CC) $(CFLAGS) -o mytestlist list_adders.o list_movers.o list_removers.o mytestlist.o

	  
mytestlist.o: mytestlist.c
	$(CC) $(CFLAGS) -c mytestlist.c

list_adders.o: list_adders.c 
	$(CC) $(CFLAGS) -c list_adders.c 

list_movers.o: list_movers.c
	$(CC) $(CFLAGS) -c list_movers.c


list_removers.o: list_removers.c
	$(CC) $(CFLAGS) -c list_removers.c


monitor.o: builddirs monitor.c monitor.h list.h
	$(CC) $(CFLAGS) -c monitor.c -o $(OBJ_LIB)/monitor.o -I. $(RTTIC) $(EXTRA)

reader-writer-monitor.o: builddirs reader-writer-monitor.c reader_writer.h monitor.h
	$(CC) $(CFLAGS) -c reader-writer-monitor.c -o $(OBJ_LIB)/reader-writer-monitor.o -I. $(RTTIC) $(EXTRA)
	
reader-writer.o: builddirs reader-writer.c reader_writer.h
	$(CC) $(CFLAGS) -c reader-writer.c -o $(OBJ_LIB)/reader-writer.o -I. $(RTTIC) $(EXTRA)
	
libmonitor.a: builddirs monitor.c
	ar cr $(LOCAL_LIB)/libmonitor.a -I. $(OBJ_LIB)/monitor.o
	
reader-writer-test: libmonitor.a liblist.a reader-writer.o reader-writer-monitor.o
	$(CC) $(CFLAGS) -o reader-writer-test.c $(OBJ_LIB)/reader-writer.o $(OBJ_LIB)/reader-writer-monitor.o $(HONLIB) $(LISTLIB) $(RTTLC) $(RTTLCUTILS)
	
	
s-chatS: liblist32.a s-chatS.o
	
	
clean:
	  rm -f *.o $(Target)
