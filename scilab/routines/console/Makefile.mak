#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/console.lib


OBJSC =   dynamic_menus.obj history.obj  zzledt.obj

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

history.obj: history.c history.h ../stack-c.h
zzledt.obj: zzledt.c ../machine.h
dynamic_menus.obj: dynamic_menus.c ../machine.h
