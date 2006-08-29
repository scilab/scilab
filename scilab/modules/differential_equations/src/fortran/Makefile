SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/integ.a

OBJSC =

OBJSF = ainvg.o bnorm.o cfode.o  ewset.o fnorm.o intdy.o lsoda.o lsode.o \
	lsodi.o prepj.o prepji.o prja.o rscma1.o rscom1.o solsy.o stoda.o \
	stode.o stodi.o svcma1.o svcom1.o vmnorm.o vnorm.o xerrwv.o xsetf.o \
	xsetun.o dqag0.o dqags.o epsalg.o quarul.o order.o \
	rgk4.o lsdisc.o lsodar.o rscar1.o svcar1.o rchek.o roots.o ddassl.o \
	colnew.o ddasrt.o rkf45.o rksimp.o twodq.o dcutet.o daux.o ddaskr.o \
	lsodar2.o rchek2.o roots2.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

ddassl.o: ../stack.h
lsdisc.o: ../stack.h
xerrwv.o: ../stack.h