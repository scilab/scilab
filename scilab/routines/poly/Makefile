SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/poly.a

OBJSC =

OBJSF = bezout.o blktit.o\
	dimin.o dmdsp.o dmdspf.o dmpad.o dmpcnc.o \
	dmpdsp.o dmpext.o dmpins.o dmpmu.o dmptld.o dmptra.o dmrdsp.o \
	dpmul.o dpmul1.o dpodiv.o dprxc.o dpsimp.o dwmpmu.o fmt.o \
	horner.o idegre.o impcnc.o impext.o impins.o imptra.o  \
	matra.o mpdiag.o mpinsp.o mptri.o \
	residu.o sfact1.o sfact2.o strdsp.o\
	wdmpad.o wdmpmu.o wmdsp.o wmpad.o wmpcnc.o wmpins.o \
	wmpmu.o wmptld.o wmptra.o wpmul.o wpmul1.o wpodiv.o wprxc.o \
	mpdegr.o dmp2pm.o recbez.o dmpadj.o wmpadj.o bezstp.o \
	dmpcle.o wmpcle.o ddmpev.o

OBJS = $(OBJSC) $(OBJSF)

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
