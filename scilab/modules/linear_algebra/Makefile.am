##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

LINEAR_ALGEBRA_FORTRAN_SOURCES = src/fortran/intdggbal.f \
src/fortran/intzgeqpf4.f \
src/fortran/intzheev.f \
src/fortran/intzgehrd.f \
src/fortran/intzgesvd1.f \
src/fortran/intzfschur.f \
src/fortran/intzgesvd2.f \
src/fortran/intdgges.f \
src/fortran/intdgeev.f \
src/fortran/intgschur.f \
src/fortran/intdgesv3.f \
src/fortran/intdggev.f \
src/fortran/intdgesv4.f \
src/fortran/DGELSY1.f \
src/fortran/intozgschur.f \
src/fortran/intdpotrf.f \
src/fortran/intdoldsvd.f \
src/fortran/intdgecon.f \
src/fortran/zoldqr.f \
src/fortran/intzgetrf.f \
src/fortran/intoschur.f \
src/fortran/complexify.f \
src/fortran/SELECT.f \
src/fortran/intzgebal.f \
src/fortran/intzgetri.f \
src/fortran/intzggbal.f \
src/fortran/intdgees0.f \
src/fortran/intdgees1.f \
src/fortran/intogschur.f \
src/fortran/intzgges.f \
src/fortran/intddet.f \
src/fortran/intzgeev.f \
src/fortran/intzgesv3.f \
src/fortran/intzgesv4.f \
src/fortran/intzggev.f \
src/fortran/intzpotrf.f \
src/fortran/intzgecon.f \
src/fortran/intzgschur.f \
src/fortran/intzoldsvd.f \
src/fortran/intdgelsy.f \
src/fortran/intdgeqpf3.f \
src/fortran/intfschur.f \
src/fortran/intdsyev.f \
src/fortran/intdgeqpf4.f \
src/fortran/intdgesvd1.f \
src/fortran/intdgehrd.f \
src/fortran/intzgees0.f \
src/fortran/intdgesvd2.f \
src/fortran/intzgees1.f \
src/fortran/intzdet.f \
src/fortran/doldqr.f \
src/fortran/scischur.f \
src/fortran/intdgetrf.f \
src/fortran/intzgelsy.f \
src/fortran/intdgetri.f \
src/fortran/intdgebal.f \
src/fortran/intzschur.f \
src/fortran/intzgeqpf3.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_backslash.c \
sci_gateway/c/sci_hess.c \
sci_gateway/c/sci_rcond.c \
sci_gateway/c/sci_bdiag.c \
sci_gateway/c/sci_slash.c \
sci_gateway/c/gw_linear_algebra2.c \
sci_gateway/c/sci_det.c \
sci_gateway/c/sci_lu.c \
sci_gateway/c/sci_balanc.c \
sci_gateway/c/sci_qr.c \
sci_gateway/c/sci_schur.c \
sci_gateway/c/sci_chol.c \
sci_gateway/c/sci_spec.c \
sci_gateway/c/gw_linear_algebra.c \
sci_gateway/c/sci_inv.c \
sci_gateway/c/sci_svd.c \
sci_gateway/c/sci_lsq.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/Ex-schur.f \
sci_gateway/fortran/bschur.f

liblinear_algebra_la_includedir=$(includedir)/linear_algebra

liblinear_algebra_la_include_HEADERS = includes/gw_linear_algebra2.h  \
	includes/gw_linear_algebra.h


liblinear_algebra_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = liblinear_algebra.la

liblinear_algebra_la_SOURCES = $(LINEAR_ALGEBRA_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

liblinear_algebra_la_DEPENDENCIES = $(top_builddir)/libs/lapack/liblapack.la \
$(top_builddir)/libs/blas/libblas.la \
$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la  

liblinear_algebra_la_LIBADD = $(liblinear_algebra_la_DEPENDENCIES)

liblinear_algebra_la_PKGCONFIG = linear_algebra.pc
