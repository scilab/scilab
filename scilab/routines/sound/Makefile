SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/sound.a

OBJSC = sound.o raw.o wav.o  sox.o libst.o misc.o fileio.o soundI.o

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

$(OBJSC):	st.h 

soundII.o: ../stack.h
