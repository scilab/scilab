#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/rand.lib

OBJSC = Rand.obj mt.obj kiss.obj clcg2.obj clcg4.obj urand.obj igngeom.obj ultra.obj fsultra.obj

OBJSF =  genbet.obj genchi.obj genexp.obj genf.obj gengam.obj genmn.obj genmul.obj gennch.obj gennf.obj gennor.obj genprm.obj ignbin.obj ignnbn.obj ignpoi.obj lennob.obj phrtsd.obj sdot.obj setgmn.obj sexpo.obj sgamma.obj snorm.obj spofa.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


genf.obj:      ../stack.h
gennf.obj:      ../stack.h



