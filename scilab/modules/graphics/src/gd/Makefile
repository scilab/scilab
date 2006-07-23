SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/gd.a

OBJSC = gd.o bdftogd.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

bdftogd.o: bdftogd.c gd.h ../machine.h
gd.o: gd.c gd.h ../machine.h mtables.c
giftogd.o: giftogd.c gd.h ../machine.h
mathmake.o: mathmake.c
mtables.o: mtables.c
webgif.o: webgif.c gd.h ../machine.h
xfonts.o: xfonts.c
