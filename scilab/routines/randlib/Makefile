SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/rand.a

OBJSC = Rand.o

OBJSF = advnst.o genbet.o genchi.o genexp.o genf.o gengam.o genmn.o genmul.o gennch.o gennf.o gennor.o genprm.o genunf.o getcgn.o getsd.o ignbin.o ignlgi.o ignnbn.o ignpoi.o ignuin.o initgn.o inrgcm.o lennob.o mltmod.o phrtsd.o qrgnin.o ranf.o sdot.o setall.o setant.o setgmn.o setsd.o sexpo.o sgamma.o snorm.o spofa.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


advnst.o:      ../stack.h
genf.o:      ../stack.h
gennf.o:      ../stack.h
getcgn.o:      ../stack.h
getsd.o:      ../stack.h
ignuin.o:      ../stack.h
initgn.o:      ../stack.h
mltmod.o:      ../stack.h
setant.o:      ../stack.h


