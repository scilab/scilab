SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/blas.a

OBJSC = 

OBJSF = dasum.o daxpy.o dcopy.o ddot.o dgbmv.o dgemm.o dgemv.o\
	dger.o dnrm2.o drot.o drotg.o dsbmv.o dscal.o dspmv.o\
	dspr.o dspr2.o dswap.o dsymm.o dsymv.o dsyr.o dsyr2.o\
	dsyr2k.o dsyrk.o dtbmv.o dtbsv.o dtpmv.o dtpsv.o\
	dtrmm.o dtrmv.o dtrsm.o dtrsv.o dzasum.o dznrm2.o idamax.o
include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
