#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/sound.lib

OBJSC = sound.obj raw.obj wav.obj  sox.obj libst.obj misc.obj fileio.obj soundI.obj

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.libmk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

$(OBJSC):	st.h 

soundII.obj: ../stack.h
