SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/maxplus.a

OBJSC = FordBellman.o Utilities2.o Karp4.o HOWARD7.o \
in_span.o Display2.o Link_Howard3.o

OBJSF = mpkarp.o mpalgtyp.o mpdspasp.o mpdsmsp.o mpdspms.o mpdspmsp.o \
mpdscal.o mpdspdrsp.o mpdspful.o mpwspful.o mpadd.o mndsmsp.o \
mpcommon.o mpdmmul.o mpwmmul.o mpwscal.o mnwmmul.o mndmmul.o \
mdadd.o mpddot.o mpwmul.o mnddot.o mndadd.o mpdadd.o mpdipowe.o \
mpddpowe.o mpddpow1.o mpdspis.o mpwspis.o mpaddmul.o mpkronr.o \
mpchsgn.o mpddif.o  mpprint.o mpdspmas.o impelm.o impmatops.o impspops.o impspelm.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


impelm.o: ../stack.h

impmatops: ../stack.h

impspops: ../stack.h

impspelm: ../stack.h

Link_Howard3: ../stack-c.h