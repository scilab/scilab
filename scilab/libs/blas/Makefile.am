##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

BLAS_FORTRAN_SOURCES = zrotg.f \
zhpr2.f \
zher2k.f \
dspr.f \
xerbla.f \
dcopy.f \
dsyr2k.f \
zsymm.f \
zhemm.f \
dtbsv.f \
dtrmm.f \
dscal.f \
ddot.f \
dgbmv.f \
dtpsv.f \
dtrsv.f \
dgemv.f \
idamax.f \
dzasum.f \
zcopy.f \
zher.f \
drot.f \
ztbsv.f \
dasum.f \
ztrmm.f \
dsbmv.f \
zscal.f \
dswap.f \
zdotc.f \
zgbmv.f \
ztpsv.f \
zgemv.f \
ztrsv.f \
izamax.f \
dspmv.f \
dcabs1.f \
dsymv.f \
zswap.f \
zdotu.f \
zgerc.f \
dznrm2.f \
dtbmv.f \
zdscal.f \
dger.f \
dnrm2.f \
zhpr.f \
daxpy.f \
zhbmv.f \
zhemv.f \
dtrsm.f \
dgemm.f \
dspr2.f \
dtpmv.f \
zgeru.f \
dtrmv.f \
dsyrk.f \
lsame.f \
ztbmv.f \
dsyr2.f \
zhpmv.f \
zsyr2k.f \
zaxpy.f \
zgemm.f \
drotg.f \
ztrsm.f \
ztpmv.f \
dsyr.f \
zsyrk.f \
ztrmv.f \
zherk.f \
dsymm.f \
zher2.f

pkglib_LTLIBRARIES = libsciblas.la

libsciblas_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciblas_la_SOURCES = $(BLAS_FORTRAN_SOURCES)

libsciblas_la_LIBADD =

#### Target ######
modulename=blas

include $(top_srcdir)/Makefile.incl.am

