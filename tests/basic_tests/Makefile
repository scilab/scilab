SHELL = /bin/sh

SCIDIR=..

include ../Makefile.incl


all:: info

info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	$(SCIDIR)/bin/scilab -nw -f maketests.sce

clean ::
	$(RM) *.dia tmp.* get_examples.log *.graph so_locations 
	$(RM) foo* test.wav one_man.tst test_macro_exec asave results *.bin
	$(RM) -r .libs libintertest.o libintertest.s* libintertest.la \
	libintertest.lo addinter.o addinter.lo

distclean:: clean 

FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS)

OBJSF = externals.o

linktest : $(OBJSF)

externals.o: externals.f
	$(FC) $(FFLAGS) -c externals.f  -o externals.o

clean::
	$(RM) externals.o
distclean::
	$(RM) externals.o

