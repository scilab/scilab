SHELL = /bin/sh

#include ../../../Path.incl
#include ../../../Version.incl
include ../../../Makefile.incl

FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS)


OBJS = /tmp/ener.o /tmp/npend.o /tmp/np.o

all:: $(OBJS)

/tmp/ener.o: ener.f
	$(FC) $(FFLAGS) -c ener.f  -o /tmp/ener.o

clean::
	$(RM) /tmp/ener.o
distclean::
	$(RM) /tmp/ener.o

/tmp/np.o: np.f
	$(FC) $(FFLAGS) -c np.f  -o /tmp/np.o

clean::
	$(RM) /tmp/np.o
distclean::
	$(RM) /tmp/np.o

/tmp/npend.o: npend.f
	$(FC) $(FFLAGS) -c npend.f -o /tmp/npend.o

distclean::
	$(RM) /tmp/npend.o
clean::
	$(RM) /tmp/npend.o
