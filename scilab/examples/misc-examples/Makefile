SHELL = /bin/sh

SCIDIR=../..
include ../../Makefile.incl

all:: info

info:
	@echo "Type \"make /tmp/pgm.o\" to compile pgm program"


FFLAGS = $(FC_OPTIONS) $(FC_PICFLAGS)

CFLAGS = $(CC_OPTIONS) $(CC_PICFLAGS)

/tmp/wfex.o: wfex.f
	$(FC) $(FFLAGS) -c wfex.f  -o /tmp/wfex.o

/tmp/wjex.o: wjex.f
	$(FC) $(FFLAGS) -c wjex.f  -o /tmp/wjex.o

/tmp/wfexab.o: wfexab.f
	$(FC) $(FFLAGS) -c wfexab.f  -o /tmp/wfexab.o

/tmp/res22.o: res22.f
	$(FC) $(FFLAGS) -c res22.f  -o /tmp/res22.o

/tmp/jac22.o: jac22.f
	$(FC) $(FFLAGS) -c jac22.f  -o /tmp/jac22.o

/tmp/gr22.o: gr22.f
	$(FC) $(FFLAGS) -c gr22.f  -o /tmp/gr22.o


clean::
	@echo Cleaning misc-examples
	@$(RM) /tmp/res22.o /tmp/jac22.o /tmp/gr22.o /tmp/fexab2.o so_locations *.o

distclean:: clean


##----------------------  test all the example  

EXAMPLES= dasrt1.sce dasrt2.sce dasrt3.sce dassl1.sce dassl2.sce ode1.sce ode2.sce ode3.sce\
	odedc.sce odeoptions.sce

tests	: $(EXAMPLES)
	@cat *.sce > zlink.tst 		
	@$(SCIDIR)/util/scidem $(SCIDIR) zlink.tst zlink.dia

clean	::
	@$(RM) zlink.dia zlink.tst

distclean::
	@$(RM) zlink.dia zlink.tst $(OBJSC) $(OBJSF)


