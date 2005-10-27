#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/blas.lib

OBJSC = 

OBJSF = dasum.obj daxpy.obj dcopy.obj ddot.obj dgbmv.obj dgemm.obj dgemv.obj\
	dger.obj dnrm2.obj drot.obj drotg.obj dsbmv.obj dscal.obj dspmv.obj\
	dspr.obj dspr2.obj dswap.obj dsymm.obj dsymv.obj dsyr.obj dsyr2.obj\
	dsyr2k.obj dsyrk.obj dtbmv.obj dtbsv.obj dtpmv.obj dtpsv.obj\
	dtrmm.obj dtrmv.obj dtrsm.obj dtrsv.obj dzasum.obj dznrm2.obj idamax.obj \
	zaxpy.obj zcopy.obj zdotc.obj zdotu.obj zdscal.obj zgbmv.obj zgemm.obj \
	zgemv.obj zgerc.obj zgeru.obj zhbmv.obj zhemm.obj zhemv.obj zher.obj \
	zher2.obj zher2k.obj zherk.obj zhpmv.obj zhpr.obj zhpr2.obj zrotg.obj \
	zscal.obj zswap.obj zsymm.obj zsyr2k.obj zsyrk.obj ztbmv.obj ztbsv.obj \
	ztpmv.obj ztpsv.obj ztrmm.obj ztrmv.obj ztrsm.obj ztrsv.obj izamax.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

