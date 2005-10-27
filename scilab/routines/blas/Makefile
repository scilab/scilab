SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/blas.a

OBJSC = 

OBJSF = dasum.o daxpy.o dcopy.o ddot.o dgbmv.o dgemm.o dgemv.o\
	dger.o dnrm2.o drot.o drotg.o dsbmv.o dscal.o dspmv.o\
	dspr.o dspr2.o dswap.o dsymm.o dsymv.o dsyr.o dsyr2.o\
	dsyr2k.o dsyrk.o dtbmv.o dtbsv.o dtpmv.o dtpsv.o\
	dtrmm.o dtrmv.o dtrsm.o dtrsv.o dzasum.o dznrm2.o idamax.o \
	zaxpy.o zcopy.o zdotc.o zdotu.o zdscal.o zgbmv.o zgemm.o \
	zgemv.o zgerc.o zgeru.o zhbmv.o zhemm.o zhemv.o zher.o \
	zher2.o zher2k.o zherk.o zhpmv.o zhpr.o zhpr2.o zrotg.o \
	zscal.o zswap.o zsymm.o zsyr2k.o zsyrk.o ztbmv.o ztbsv.o \
	ztpmv.o ztpsv.o ztrmm.o ztrmv.o ztrsm.o ztrsv.o izamax.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib
