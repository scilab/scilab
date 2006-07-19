SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/sound.a

OBJSC = gw_sound.o sci_loadwave.o sci_savewave.o sci_beep.o sci_PlaySound.o wav.o raw.o sox.o libst.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(TK_INC_PATH) $(TCL_INC_PATH) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

