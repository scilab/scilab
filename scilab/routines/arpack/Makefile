SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/arpack.a

OBJSC =

OBJSF  = dgetv0.o dlaqrb.o dstqrb.o dsortc.o dsortr.o dstatn.o dstats.o \
	dnaitr.o dnapps.o dnaup2.o dnaupd.o dnconv.o dneigh.o dngets.o \
	dsaitr.o dsapps.o dsaup2.o dsaupd.o dsconv.o dseigt.o dsgets.o \
	dneupd.o dseupd.o dsesrt.o \
	ivout.o dvout.o dmout.o	dlarnv.o dlaruv.o second.o \
	znaupd.o \
	znaitr.o znapps.o znaup2.o \
	zneigh.o zneupd.o zngets.o \
	zgetv0.o zsortc.o zstatn.o \
	zvout.o zmout.o \
	zlarnv.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)
FFLAGS = $(FC_OPTIONS)

include ../Make.lib

dgetv0.o: debug.h stat.h
dnaitr.o: debug.h stat.h
dnapps.o: debug.h stat.h
dnaup2.o: debug.h stat.h
dnaupd.o: debug.h stat.h
dnconv.o: debug.h stat.h
dneigh.o: debug.h stat.h
dneupd.o: debug.h stat.h
dngets.o: debug.h stat.h
dsaitr.o: debug.h stat.h
dsapps.o: debug.h stat.h
dsaup2.o: debug.h stat.h
dsaupd.o: debug.h stat.h
dsconv.o: debug.h stat.h
dseigt.o: debug.h stat.h
dseupd.o: debug.h stat.h
dsgets.o: debug.h stat.h
dstatn.o: stat.h
dstats.o: stat.h
zgetv0.o: debug.h stat.h
znaitr.o: debug.h stat.h
znapps.o: debug.h stat.h
znaup2.o: debug.h stat.h
znaupd.o: debug.h stat.h
zneigh.o: debug.h stat.h
zneupd.o: debug.h stat.h
zngets.o: debug.h stat.h
zstatn.o: stat.h
