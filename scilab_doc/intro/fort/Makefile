SHELL = /bin/sh

#include ../../../Path.incl
#include ../../../Version.incl
include ../../../Makefile.incl

OBJS = /tmp/fibb.o

all:: $(OBJS)

/tmp/fibb.o: fibb.f
	$(FC) $(FFLAGS) -c fibb.f -o /tmp/fibb.o

clean::
	$(RM) /tmp/fibb.o
distclean::
	$(RM) /tmp/fibb.o

