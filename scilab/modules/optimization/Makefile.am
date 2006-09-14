OPTIMIZATION_C_SOURCES = src/c/intqld.c \
src/c/sp.c

OPTIMIZATION_FORTRAN_SOURCES = src/fortran/dimp03.f \
src/fortran/qrfac.f \
src/fortran/gcbd.f \
src/fortran/n1qn2a.f \
src/fortran/dogleg.f \
src/fortran/anrs02.f \
src/fortran/fuclid.f \
src/fortran/fmc11b.f \
src/fortran/opvf03.f \
src/fortran/ql0001.f \
src/fortran/aux003.f \
src/fortran/shanph.f \
src/fortran/fajc1.f \
src/fortran/majysa.f \
src/fortran/fdjac2.f \
src/fortran/icsei.f \
src/fortran/dipvtf.f \
src/fortran/majour.f \
src/fortran/n1qn1.f \
src/fortran/hybrj1.f \
src/fortran/n1qn1a.f \
src/fortran/n1qn3.f \
src/fortran/fmc11z.f \
src/fortran/tol03.f \
src/fortran/icse0.f \
src/fortran/icse2.f \
src/fortran/fmuls1.f \
src/fortran/icsec2.f \
src/fortran/gcp.f \
src/fortran/n1fc1a.f \
src/fortran/n1gc2.f \
src/fortran/proj.f \
src/fortran/n1gc2a.f \
src/fortran/ddd2.f \
src/fortran/fremf2.f \
src/fortran/anfm01.f \
src/fortran/dcube.f \
src/fortran/anfm03.f \
src/fortran/fcube.f \
src/fortran/bfgsd.f \
src/fortran/fcomp1.f \
src/fortran/fmulb1.f \
src/fortran/anfm05.f \
src/fortran/qrsolv.f \
src/fortran/n1fc1o.f \
src/fortran/relvar.f \
src/fortran/satur.f \
src/fortran/strang.f \
src/fortran/desr03.f \
src/fortran/anrs01.f \
src/fortran/plcbas.f \
src/fortran/hybrd.f \
src/fortran/fmc11a.f \
src/fortran/pasr03.f \
src/fortran/fmani1.f \
src/fortran/fmc11e.f \
src/fortran/auxo01.f \
src/fortran/hybrj.f \
src/fortran/zqnbd.f \
src/fortran/r1updt.f \
src/fortran/ajour.f \
src/fortran/dnrm0.f \
src/fortran/dpmpar.f \
src/fortran/fretc1.f \
src/fortran/qnbd.f \
src/fortran/nlis0.f \
src/fortran/nvkt03.f \
src/fortran/optr01.f \
src/fortran/nlis2.f \
src/fortran/optr03.f \
src/fortran/fdjac1.f \
src/fortran/hybrd1.f \
src/fortran/lmdif.f \
src/fortran/n1qn2.f \
src/fortran/zthz.f \
src/fortran/majz.f \
src/fortran/n1qn3a.f \
src/fortran/frdf1.f \
src/fortran/lmder.f \
src/fortran/icse1.f \
src/fortran/ctonb.f \
src/fortran/intlsqrsolve.f \
src/fortran/icscof.f \
src/fortran/fmlag1.f \
src/fortran/fpq2.f \
src/fortran/n1fc1.f \
src/fortran/ffinf1.f \
src/fortran/lmpar.f \
src/fortran/ctcab.f \
src/fortran/n1gc2b.f \
src/fortran/fprf2.f \
src/fortran/fremf1.f \
src/fortran/enorm.f \
src/fortran/icse.f \
src/fortran/anfm02.f \
src/fortran/rlbd.f \
src/fortran/qform.f \
src/fortran/anfm04.f \
src/fortran/calmaj.f \
src/fortran/anfm06.f \
src/fortran/calbx.f \
src/fortran/r1mpyq.f \
src/fortran/rednor.f \
src/fortran/zgcbd.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_semidef.c \
sci_gateway/c/sci_qld.c \
sci_gateway/c/gw_optim.c \
sci_gateway/c/sci_quapro.c \
sci_gateway/c/sci_optim.c \
sci_gateway/c/sci_fsolv.c \
sci_gateway/c/sci_sqrsolve.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_semidef.f \
sci_gateway/fortran/sci_f_fsolve.f \
sci_gateway/fortran/sci_f_quapro.f \
sci_gateway/fortran/sci_f_optim.f


liboptimization_la_includedir=$(includedir)/optimization

liboptimization_la_include_HEADERS = includes/gw_optim.h  \
includes/spd.h


liboptimization_la_CFLAGS=  -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = liboptimization.la

liboptimization_la_SOURCES = $(OPTIMIZATION_C_SOURCES) $(OPTIMIZATION_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

liboptimization_la_DEPENDENCIES = \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/arnoldi/libarnoldi.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 



liboptimization_la_LIBADD = $(liboptimization_la_DEPENDENCIES)

liboptimization_la_PKGCONFIG = optimization.pc
