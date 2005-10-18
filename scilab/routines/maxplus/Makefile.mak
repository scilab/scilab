#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/maxplus.lib

OBJSC = FordBellman.obj Utilities2.obj Karp4.obj HOWARD7.obj \
in_span.obj Display2.obj Link_Howard3.obj

OBJSF = mpkarp.obj mpalgtyp.obj mpdspasp.obj mpdsmsp.obj mpdspms.obj mpdspmsp.obj \
mpdscal.obj mpdspdrsp.obj mpdspful.obj mpwspful.obj mpadd.obj mndsmsp.obj \
mpcommon.obj mpdmmul.obj mpwmmul.obj mpwscal.obj mnwmmul.obj mndmmul.obj \
mdadd.obj mpddot.obj mpwmul.obj mnddot.obj mndadd.obj mpdadd.obj mpdipowe.obj \
mpddpowe.obj mpddpow1.obj mpdspis.obj mpwspis.obj mpaddmul.obj mpkronr.obj \
mpchsgn.obj mpddif.obj  mpprint.obj mpdspmas.obj impelm.obj impmatops.obj impspops.obj impspelm.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


impelm.obj: ../stack.h

impmatops: ../stack.h

impspops: ../stack.h

impspelm: ../stack.h

Link_Howard3: ../stack-c.h