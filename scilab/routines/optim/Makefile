SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/optim.a

OBJSC = sp.o 

OBJSF =	ajour.o bfgsd.o calbx.o calmaj.o fmulb1.o fmuls1.o fpq2.o \
	fremf1.o gcbd.o gcp.o  majysa.o majour.o majz.o \
	n1fc1.o n1fc1a.o ffinf1.o fprf2.o fremf2.o n1fc1o.o frdf1.o \
	fuclid.o nlis2.o \
	n1gc2.o n1gc2a.o n1gc2b.o n1qn1.o n1qn1a.o \
	proj.o qnbd.o rednor.o relvar.o rlbd.o satur.o shanph.o optr03.o \
	optr01.o zgcbd.o zqnbd.o n1qn2.o n1qn2a.o strang.o plcbas.o \
	anfm01.o anfm02.o anfm03.o anfm04.o anfm05.o anfm06.o \
	anrs01.o anrs02.o \
	aux003.o auxo01.o opvf03.o tol03.o desr03.o dimp03.o nvkt03.o dnrm0.o \
	pasr03.o dipvtf.o icse.o icse0.o icse1.o icse2.o zthz.o icscof.o \
	icsei.o icsec2.o \
	dogleg.o enorm.o fdjac1.o fdjac2.o hybrd.o hybrd1.o hybrj.o hybrj1.o \
	dpmpar.o lmder.o lmdif.o lmpar.o qrsolv.o \
	qform.o qrfac.o r1mpyq.o r1updt.o\
	fmc11z.o fajc1.o fretc1.o fmani1.o fcomp1.o \
	fmlag1.o nlis0.o fcube.o fmc11a.o fmc11b.o fmc11e.o \
	n1qn3.o n1qn3a.o ddd2.o dcube.o ctonb.o ctcab.o ql0001.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)
FFLAGS = $(FC_OPTIONS)

include ../Make.lib

sp.o: spd.h ../machine.h
