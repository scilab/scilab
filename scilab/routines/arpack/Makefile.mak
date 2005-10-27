#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/arpack.lib

OBJSC =

OBJSF  = dgetv0.obj dlaqrb.obj dstqrb.obj dsortc.obj dsortr.obj dstatn.obj dstats.obj \
	dnaitr.obj dnapps.obj dnaup2.obj dnaupd.obj dnconv.obj dneigh.obj dngets.obj \
	dsaitr.obj dsapps.obj dsaup2.obj dsaupd.obj dsconv.obj dseigt.obj dsgets.obj \
	dneupd.obj dseupd.obj dsesrt.obj \
	ivout.obj dvout.obj dmout.obj	dlarnv.obj dlaruv.obj second.obj \
	znaupd.obj \
	znaitr.obj znapps.obj znaup2.obj \
	zneigh.obj zneupd.obj zngets.obj \
	zgetv0.obj zsortc.obj zstatn.obj \
	zvout.obj zmout.obj \
	zlarnv.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

dgetv0.obj: debug.h stat.h
dnaitr.obj: debug.h stat.h
dnapps.obj: debug.h stat.h
dnaup2.obj: debug.h stat.h
dnaupd.obj: debug.h stat.h
dnconv.obj: debug.h stat.h
dneigh.obj: debug.h stat.h
dneupd.obj: debug.h stat.h
dngets.obj: debug.h stat.h
dsaitr.obj: debug.h stat.h
dsapps.obj: debug.h stat.h
dsaup2.obj: debug.h stat.h
dsaupd.obj: debug.h stat.h
dsconv.obj: debug.h stat.h
dseigt.obj: debug.h stat.h
dseupd.obj: debug.h stat.h
dsgets.obj: debug.h stat.h
dstatn.obj: stat.h
dstats.obj: stat.h
zgetv0.obj: debug.h stat.h
znaitr.obj: debug.h stat.h
znapps.obj: debug.h stat.h
znaup2.obj: debug.h stat.h
znaupd.obj: debug.h stat.h
zneigh.obj: debug.h stat.h
zneupd.obj: debug.h stat.h
zngets.obj: debug.h stat.h
zstatn.obj: stat.h
