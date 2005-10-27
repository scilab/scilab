#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/gd.lib

OBJSC = gd.obj bdftogd.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

bdftogd.obj: bdftogd.c gd.h ../machine.h
gd.obj: gd.c gd.h ../machine.h mtables.c
giftogd.obj: giftogd.c gd.h ../machine.h
mathmake.obj: mathmake.c
mtables.obj: mtables.c
webgif.obj: webgif.c gd.h ../machine.h
xfonts.obj: xfonts.c
