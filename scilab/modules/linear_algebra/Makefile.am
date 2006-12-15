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

liblinear_algebra_la_CFLAGS=  -Iincludes/ 

lib_LTLIBRARIES = liblinear_algebra.la

liblinear_algebra_la_SOURCES = $(LINEAR_ALGEBRA_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

liblinear_algebra_la_LIBADD = $(top_builddir)/libs/lapack/liblapack.la \
$(top_builddir)/libs/blas/libblas.la \
$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la  

liblinear_algebra_la_PKGCONFIG = linear_algebra.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/linear_algebra


#### linear_algebra : Conf files ####
liblinear_algebra_la_rootdir = $(target)
liblinear_algebra_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### linear_algebra : Scilab macros ###
liblinear_algebra_la_macrodir = $(target)/macros
liblinear_algebra_la_macro_DATA = macros/spaninter.sci \
macros/WritemiMatrix.sci \
macros/penlaur.sci \
macros/ndims.sci \
macros/norm.sci \
macros/pinv.sci \
macros/coff.sci \
macros/randpencil.sci \
macros/kernel.sci \
macros/pbig.sci \
macros/im_inv.sci \
macros/householder.sci \
macros/companion.sci \
macros/cond.sci \
macros/projspec.sci \
macros/quaskro.sci \
macros/classmarkov.sci \
macros/ReadmiMatrix.sci \
macros/hypermat.sci \
macros/sva.sci \
macros/fullrf.sci \
macros/linsolve.sci \
macros/kroneck.sci \
macros/colcomp.sci \
macros/range.sci \
macros/rref.sci \
macros/gspec.sci \
macros/chsolve.sci \
macros/rowshuff.sci \
macros/nlev.sci \
macros/pencan.sci \
macros/fullrfk.sci \
macros/spanplus.sci \
macros/eigenmarkov.sci \
macros/diff.sci \
macros/pen2ea.sci \
macros/proj.sci \
macros/orth.sci \
macros/polar.sci \
macros/aff2ab.sci \
macros/glever.sci \
macros/rowcomp.sci \
macros/sqroot.sci \
macros/chfact.sci \
macros/psmall.sci \
macros/genmarkov.sci \
macros/gschur.sci \
macros/givens.sci \
macros/rank.sci \
macros/spantwo.sci \
macros/trace.sci macros/aff2ab.bin \
macros/chfact.bin \
macros/chsolve.bin \
macros/classmarkov.bin \
macros/coff.bin \
macros/colcomp.bin \
macros/companion.bin \
macros/cond.bin \
macros/diff.bin \
macros/eigenmarkov.bin \
macros/fullrfk.bin \
macros/fullrf.bin \
macros/genmarkov.bin \
macros/givens.bin \
macros/glever.bin \
macros/gschur.bin \
macros/gspec.bin \
macros/householder.bin \
macros/hypermat.bin \
macros/im_inv.bin \
macros/kernel.bin \
macros/kroneck.bin \
macros/linsolve.bin \
macros/ndims.bin \
macros/nlev.bin \
macros/norm.bin \
macros/orth.bin \
macros/pbig.bin \
macros/pen2ea.bin \
macros/pencan.bin \
macros/penlaur.bin \
macros/pinv.bin \
macros/polar.bin \
macros/proj.bin \
macros/projspec.bin \
macros/psmall.bin \
macros/quaskro.bin \
macros/randpencil.bin \
macros/range.bin \
macros/rank.bin \
macros/ReadmiMatrix.bin \
macros/rowcomp.bin \
macros/rowshuff.bin \
macros/rref.bin \
macros/spaninter.bin \
macros/spanplus.bin \
macros/spantwo.bin \
macros/sqroot.bin \
macros/sva.bin \
macros/trace.bin \
macros/WritemiMatrix.bin macros/names macros/lib macros/buildmacros.sce


#### linear_algebra : init scripts #### 
liblinear_algebra_la_etcdir = $(target)/etc
liblinear_algebra_la_etc_DATA = etc/linear_algebra.quit etc/linear_algebra.start
#### linear_algebra : gateway declaration #### 
liblinear_algebra_la_sci_gatewaydir = $(target)/sci_gateway
liblinear_algebra_la_sci_gateway_DATA = sci_gateway/linear_algebra

#### linear_algebra : documentation FR #### 
liblinear_algebra_la_helpfrdir = $(target)/help/fr
liblinear_algebra_la_helpfr_DATA = help/fr/eigenmarkov.htm \
help/fr/ereduc.htm \
help/fr/fstair.htm \
help/fr/genmarkov.htm \
help/fr/gschur.htm \
help/fr/im_inv.htm \
help/fr/kroneck.htm \
help/fr/lsq.htm \
help/fr/pencan.htm \
help/fr/penlaur.htm \
help/fr/projspec.htm \
help/fr/psmall.htm \
help/fr/quaskro.htm \
help/fr/randpencil.htm \
help/fr/rankqr.htm \
help/fr/rowshuff.htm \
help/fr/rref.htm \
help/fr/schur.htm \
help/fr/spaninter.htm \
help/fr/spanplus.htm \
help/fr/spantwo.htm \
help/fr/sylv.htm \
help/fr/bdiag.htm \
help/fr/chfact.htm \
help/fr/whatis.htm \
help/fr/balanc.htm \
help/fr/aff2ab.htm \
help/fr/classmarkov.htm \
help/fr/chol.htm \
help/fr/chsolve.htm \
help/fr/coff.htm \
help/fr/colcomp.htm \
help/fr/companion.htm \
help/fr/cond.htm \
help/fr/det.htm \
help/fr/expm.htm \
help/fr/exp.htm \
help/fr/fullrfk.htm \
help/fr/fullrf.htm \
help/fr/givens.htm \
help/fr/glever.htm \
help/fr/gspec.htm \
help/fr/hess.htm \
help/fr/householder.htm \
help/fr/inv.htm \
help/fr/kernel.htm \
help/fr/linsolve.htm \
help/fr/lu.htm \
help/fr/lyap.htm \
help/fr/nlev.htm \
help/fr/orth.htm \
help/fr/pbig.htm \
help/fr/pinv.htm \
help/fr/polar.htm \
help/fr/proj.htm \
help/fr/qr.htm \
help/fr/range.htm \
help/fr/rank.htm \
help/fr/rcond.htm \
help/fr/rowcomp.htm \
help/fr/spec.htm \
help/fr/sqroot.htm \
help/fr/sva.htm \
help/fr/svd.htm \
help/fr/trace.htm


#### linear_algebra : documentation EN #### 
liblinear_algebra_la_helpendir = $(target)/help/eng
liblinear_algebra_la_helpen_DATA = help/eng/whatis.htm \
help/eng/linsolve.htm \
help/eng/aff2ab.htm \
help/eng/balanc.htm \
help/eng/bdiag.htm \
help/eng/chfact.htm \
help/eng/chol.htm \
help/eng/chsolve.htm \
help/eng/coff.htm \
help/eng/classmarkov.htm \
help/eng/colcomp.htm \
help/eng/cond.htm \
help/eng/companion.htm \
help/eng/det.htm \
help/eng/ereduc.htm \
help/eng/eigenmarkov.htm \
help/eng/expm.htm \
help/eng/exp.htm \
help/eng/fstair.htm \
help/eng/fullrfk.htm \
help/eng/fullrf.htm \
help/eng/givens.htm \
help/eng/genmarkov.htm \
help/eng/glever.htm \
help/eng/gschur.htm \
help/eng/gspec.htm \
help/eng/hess.htm \
help/eng/im_inv.htm \
help/eng/householder.htm \
help/eng/inv.htm \
help/eng/kernel.htm \
help/eng/kroneck.htm \
help/eng/lyap.htm \
help/eng/lsq.htm \
help/eng/lu.htm \
help/eng/projspec.htm \
help/eng/nlev.htm \
help/eng/orth.htm \
help/eng/pbig.htm \
help/eng/pencan.htm \
help/eng/penlaur.htm \
help/eng/pinv.htm \
help/eng/polar.htm \
help/eng/rowshuff.htm \
help/eng/proj.htm \
help/eng/psmall.htm \
help/eng/qr.htm \
help/eng/quaskro.htm \
help/eng/range.htm \
help/eng/randpencil.htm \
help/eng/rankqr.htm \
help/eng/rank.htm \
help/eng/rcond.htm \
help/eng/rowcomp.htm \
help/eng/spanplus.htm \
help/eng/rref.htm \
help/eng/schur.htm \
help/eng/spaninter.htm \
help/eng/spantwo.htm \
help/eng/spec.htm \
help/eng/sqroot.htm \
help/eng/sva.htm \
help/eng/svd.htm \
help/eng/sylv.htm \
help/eng/trace.htm


#### linear_algebra : include files ####
liblinear_algebra_la_includedir=$(includedir)/linear_algebra
liblinear_algebra_la_include_HEADERS = includes/gw_linear_algebra.h \
includes/gw_linear_algebra2.h

