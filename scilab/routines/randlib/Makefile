SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/rand.a

OBJSC = Rand.o mt.o kiss.o clcg2.o clcg4.o urand.o igngeom.o ultra.o fsultra.o

OBJSF =  genbet.o genchi.o genexp.o genf.o gengam.o genmn.o genmul.o gennch.o gennf.o gennor.o genprm.o ignbin.o ignnbn.o ignpoi.o lennob.o phrtsd.o sdot.o setgmn.o sexpo.o sgamma.o snorm.o spofa.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


genf.o:      ../stack.h
gennf.o:      ../stack.h



