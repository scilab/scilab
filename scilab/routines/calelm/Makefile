SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/calelm.a

OBJSC = vfloor.o vceil.o  finite.o vfrexp.o sci_tools.o

OBJSF = coshin.o arcosh.o  dad.o dadd.o  ccopy.o  rcopy.o ddif.o \
	 dmcopy.o dmmul.o dset.o dsort.o \
	dsum.o  dtild.o dvmul.o entier.o franck.o gdcp2i.o hilber.o \
	icopy.o  iset.o isort.o iwamax.o kronc.o kronr.o magic.o \
	mtran.o pythag.o  round.o simple.o urand.o wasum.o watan.o \
	waxpy.o wcopy.o wdiv.o wdotci.o wdotcr.o wlog.o wmmul.o wmul.o \
	wnrm2.o wrscal.o wscal.o wsign.o wsqrt.o wswap.o wvmul.o dipow.o \
	ddpow.o dwpow.o wdpow.o wipow.o wwpow.o int2db.o rea2db.o \
	isoval.o isova0.o imcopy.o dlblks.o rat.o lnblnk.o dmmul1.o \
	ddpowe.o dipowe.o dwpowe.o wdpowe.o wipowe.o wwpowe.o \
	ddpow1.o dwpow1.o wdpow1.o wwpow1.o \
	wwdiv.o wddiv.o dwdiv.o ddrdiv.o wwrdiv.o dwrdiv.o wdrdiv.o\
	ivimp.o intp.o cusum.o cupro.o cuproi.o rcsort.o wsort.o vpythag.o\
	calerf.o gamma.o ribesl.o rkbesl.o rjbesl.o rybesl.o psi.o dlgama.o\
	dmsum.o wmsum.o dmprod.o wmprod.o drdiv.o dcabs1.o infinity.o unsfdcopy.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

finite.o: ../machine.h

sci_tools.o : sci_tools.h ../machine.h
