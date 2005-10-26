SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/signal.a

OBJSC =

OBJSF = sigelm.o chebc.o cmpse3.o dfftbi.o fft842.o freque.o kaiser.o r2tx.o rpem.o \
	cheby.o coeft.o dfftmx.o filbut.o hammin.o nstabl.o r4tx.o \
	cmpse2.o dfft2.o ino.o poles.o r8tx.o tscccf.o  \
	dpspln.o dspln.o tg02ad.o  amell.o belan.o bldenz.o blnumz.o bounn.o  \
	compel.o dgee01.o degree.o deli1.o deli11.o deli2.o delip.o dellk.o  \
	desi00.o desi01.o desi11.o desi12.o desi14.o desi21.o desi22.o  \
	desi24.o desia.o desib.o  dsqrtc.o gee.o ouch.o parcha.o remez.o  \
	romeg.o sn.o snell.o syredi.o tranpo.o transn.o tranze.o trbipo.o  \
	trbize.o  dsn2.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

datatf.o: ../stack.h
sigelm.o: ../stack.h
