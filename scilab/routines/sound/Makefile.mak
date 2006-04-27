SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/sound.lib


OBJSC = matsound.obj intloadwave.obj intsavewave.obj intbeep.obj intPlaySound.obj wav.obj raw.obj sox.obj libst.obj


include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

