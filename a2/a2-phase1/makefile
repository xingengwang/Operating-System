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
	$(CC) $(CFLAGS) -o s-chatS $(OBJ_LIB)/s-chatS.o $(LISTLIB32) $(UBCL) $(EXTRA)

s-chatL: liblist32.a s-chatL.o
	$(CC) $(CFLAGS) -o s-chatL $(OBJ_LIB)/s-chatL.o $(LISTLIB32) $(UBCL) $(EXTRA)

s-chatS.o: builddirs s-chat.c list.h
	$(CC) $(CFLAGS) -c s-chat.c -o $(OBJ_LIB)/s-chatS.o -I. $(UBCI)

s-chatL.o: builddirs s-chat.c list.h
	$(CC) $(CFLAGS) -c s-chat.c -o $(OBJ_LIB)/s-chatL.o -I. $(UBCI)
	
clean:
	  rm -f *.o $(Target)
