SHELL = /bin/sh

include ../../../Path.incl
include ../../../Version.incl
include ../../../Makefile.incl

OBJS = /tmp/wheel.o /tmp/wheelg.o

all:: $(OBJS)

FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS)


/tmp/wheel.o: wheel.f
	$(FC) $(FFLAGS) -c wheel.f -o /tmp/wheel.o

clean::
	$(RM) /tmp/wheel.o
distclean::
	$(RM) /tmp/wheel.o

/tmp/wheelg.o: wheelg.f
	$(FC) $(FFLAGS) -c wheelg.f -o /tmp/wheelg.o

clean::
	$(RM) /tmp/wheelg.o
distclean::
	$(RM) /tmp/wheelg.o
