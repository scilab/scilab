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

FFLAGS = $(FC_OPTIONS) -Wall

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
