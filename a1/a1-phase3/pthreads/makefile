#CC      = cc    # use CC for hpux
CC     = gcc   # use gcc otherwise

#ARCH    = i386

PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM), solaris)
ARCH = $(PLATFORM)
else
ARCH = $(shell arch)
endif

#CFLAGS = -O2 -g -Di386 	#defines for old version of linux and optimized
#CFLAGS  =  -g -D$(ARCH) -Aa -D_HPUX_SOURCE #defines for hpux
CFLAGS =  -Wall -g -D$(ARCH)                    # defines otherwise

#ifeq ($(ARCH), i386)
CPP = /usr/bin/cpp
#ifeq ($(PLATFORM), Darwin)
#CPP = /usr/bin/cpp
#endif
#ifeq ($(PLATFORM), netbsd)
#CPP = /usr/bin/cpp
#endif

#endif

INCLUDEDIR=.
LIBDIR=lib

app:    app.o $(LIBDIR)/$(PLATFORM)$(ARCH)/libpthreads.a 
	$(CC) $(CFLAGS) app.o -L$(LIBDIR)/$(PLATFORM)$(ARCH) -lpthreads -o app

$(LIBDIR)/$(PLATFORM)$(ARCH)/libpthreads.a: startup.o context.o queue.o procMgmt.o ipc.o time.o scheduling.o synch.o mem.o
	ar rc libpthreads.a startup.o context.o queue.o procMgmt.o ipc.o time.o scheduling.o synch.o mem.o
	ranlib libpthreads.a
	chmod 644 libpthreads.a
	mv libpthreads.a lib/$(PLATFORM)$(ARCH)

kernel.h: standards.h os.h kernelTypes.h ipc.h ptime.h procMgmt.h mem.h synch.h scheduling.h queue.h kernelConfig.h
#	touch kernel.h

app.o: app.c os.h standards.h
	$(CC) $(CFLAGS) -c app.c -I$(INCLUDEDIR)

synch.o:  synch.c kernel.h os.h
	$(CC) $(CFLAGS) -c synch.c -o synch.o

scheduling.o:  scheduling.c kernel.h
	$(CC) $(CFLAGS) -c scheduling.c -o scheduling.o

procMgmt.o:  procMgmt.c kernel.h
	$(CC) $(CFLAGS) -c procMgmt.c -o procMgmt.o

queue.o:  queue.c kernel.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

ipc.o:  ipc.c kernel.h
	$(CC) $(CFLAGS) -c ipc.c -o ipc.o

time.o:  time.c kernel.h
	$(CC) $(CFLAGS) -c time.c -o time.o

mem.o:  mem.c kernel.h
	$(CC) $(CFLAGS) -c mem.c -o mem.o

startup.o:  startup.c kernel.h
	$(CC) $(CFLAGS) -c startup.c -o startup.o

context.o: context.s
	$(CPP) -D$(PLATFORM) -D$(ARCH) -P context.s > _context.s
	$(CC) -D$(ARCH) -g -c -o context.o _context.s
	rm _context.s

clean:
	rm *.o lib/$(PLATFORM)$(ARCH)/*.a app 
