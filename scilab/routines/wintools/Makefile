SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/wintools.a

OBJSC = intwintools.o

OBJSF =


include ../../Makefile.incl

#CFLAGS = $(CC_OPTIONS) -DNODCD -DNOMETANET -DNOSCICOS -DNOSIGNAL -DNOSOUND -DNOSPARSE

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib



all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile


intwintools.o: ../stack.h
