#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/signal.lib

OBJSC =

OBJSF = sigelm.obj chebc.obj cmpse3.obj dfftbi.obj fft842.obj freque.obj kaiser.obj r2tx.obj rpem.obj \
	cheby.obj coeft.obj dfftmx.obj filbut.obj hammin.obj nstabl.obj r4tx.obj \
	cmpse2.obj dfft2.obj ino.obj poles.obj r8tx.obj tscccf.obj  \
	dpspln.obj dspln.obj tg02ad.obj  amell.obj belan.obj bldenz.obj blnumz.obj bounn.obj  \
	compel.obj dgee01.obj degree.obj deli1.obj deli11.obj deli2.obj delip.obj dellk.obj  \
	desi00.obj desi01.obj desi11.obj desi12.obj desi14.obj desi21.obj desi22.obj  \
	desi24.obj desia.obj desib.obj  dsqrtc.obj gee.obj ouch.obj parcha.obj remez.obj  \
	romeg.obj sn.obj snell.obj syredi.obj tranpo.obj transn.obj tranze.obj trbipo.obj  \
	trbize.obj  dsn2.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

datatf.obj: ../stack.h
sigelm.obj: ../stack.h
