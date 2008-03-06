##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

LINEAR_ALGEBRA_C_SOURCES = src/c/schurtable.c \
src/c/issymmetric.c

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
src/fortran/intzgeqpf3.f \
src/fortran/writebuf.f
#src/fortran/issymmetric.f


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
sci_gateway/c/sci_eig.c \
sci_gateway/c/gw_linear_algebra.c \
sci_gateway/c/sci_inv.c \
sci_gateway/c/sci_svd.c \
sci_gateway/c/sci_lsq.c

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/Ex-schur.f \
sci_gateway/fortran/bschur.f

libscilinear_algebra_la_CFLAGS=  	-Iincludes/ \
-I$(top_builddir)/modules/output_stream/includes \
-I$(top_builddir)/modules/dynamic_link/includes

pkglib_LTLIBRARIES = libscilinear_algebra.la

libscilinear_algebra_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscilinear_algebra_la_SOURCES = $(LINEAR_ALGEBRA_FORTRAN_SOURCES) $(LINEAR_ALGEBRA_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libscilinear_algebra_la_LIBADD =  $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/dynamic_link/libscidynamic_link.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 

if USE_EMBEDDED_LAPACK
libscilinear_algebra_la_LIBADD += $(top_builddir)/libs/lapack/libscilapack.la
endif

if USE_EMBEDDED_BLAS
libscilinear_algebra_la_LIBADD += $(top_builddir)/libs/blas/libsciblas.la 
endif

#### Target ######
modulename=linear_algebra


#### linear_algebra : Conf files ####
libscilinear_algebra_la_rootdir = $(mydatadir)
libscilinear_algebra_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### linear_algebra : init scripts ####
libscilinear_algebra_la_etcdir = $(mydatadir)/etc
libscilinear_algebra_la_etc_DATA = etc/linear_algebra.quit etc/linear_algebra.start
#### linear_algebra : gateway declaration ####
libscilinear_algebra_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscilinear_algebra_la_sci_gateway_DATA = sci_gateway/linear_algebra_gateway.xml


#### linear_algebra : include files ####
libscilinear_algebra_la_includedir=$(pkgincludedir)/linear_algebra
libscilinear_algebra_la_include_HEADERS = includes/gw_linear_algebra.h \
includes/gw_linear_algebra2.h \
includes/issymmetric.h

include $(top_srcdir)/Makefile.incl.am
