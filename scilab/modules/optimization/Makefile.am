##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

OPTIMIZATION_C_SOURCES = src/c/sp.c \
src/c/fsolvetable.c \
src/c/lsqrsolvtable.c \
src/c/optimtable.c

OPTIMIZATION_FORTRAN_SOURCES = src/fortran/gcbd.f \
src/fortran/qpgen2.f \
src/fortran/n1qn2a.f \
src/fortran/fuclid.f \
src/fortran/fmc11b.f \
src/fortran/ql0001.f \
src/fortran/shanph.f \
src/fortran/fajc1.f \
src/fortran/majysa.f \
src/fortran/icsei.f \
src/fortran/majour.f \
src/fortran/n1qn1.f \
src/fortran/n1qn1a.f \
src/fortran/n1qn3.f \
src/fortran/fmc11z.f \
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
src/fortran/dcube.f \
src/fortran/fcube.f \
src/fortran/bfgsd.f \
src/fortran/fcomp1.f \
src/fortran/fmulb1.f \
src/fortran/n1fc1o.f \
src/fortran/relvar.f \
src/fortran/satur.f \
src/fortran/strang.f \
src/fortran/fmc11a.f \
src/fortran/fmani1.f \
src/fortran/fmc11e.f \
src/fortran/zqnbd.f \
src/fortran/ajour.f \
src/fortran/fretc1.f \
src/fortran/qnbd.f \
src/fortran/nlis0.f \
src/fortran/nlis2.f \
src/fortran/n1qn2.f \
src/fortran/majz.f \
src/fortran/n1qn3a.f \
src/fortran/frdf1.f \
src/fortran/icse1.f \
src/fortran/ctonb.f \
src/fortran/icscof.f \
src/fortran/fmlag1.f \
src/fortran/fpq2.f \
src/fortran/n1fc1.f \
src/fortran/ffinf1.f \
src/fortran/ctcab.f \
src/fortran/n1gc2b.f \
src/fortran/fprf2.f \
src/fortran/fremf1.f \
src/fortran/icse.f \
src/fortran/rlbd.f \
src/fortran/calmaj.f \
src/fortran/calbx.f \
src/fortran/rednor.f \
src/fortran/zgcbd.f \
src/fortran/writebuf.f \
src/fortran/qpgen1sci.f

MINPACK_SOURCES = src/fortran/minpack/lmdif.f \
src/fortran/minpack/qrfac.f \
src/fortran/minpack/hybrj1.f \
src/fortran/minpack/dogleg.f \
src/fortran/minpack/lmder.f \
src/fortran/minpack/hybrd.f \
src/fortran/minpack/dpmpar.f \
src/fortran/minpack/lmpar.f \
src/fortran/minpack/hybrj.f \
src/fortran/minpack/r1updt.f \
src/fortran/minpack/enorm.f \
src/fortran/minpack/qform.f \
src/fortran/minpack/qrsolv.f \
src/fortran/minpack/fdjac1.f \
src/fortran/minpack/hybrd1.f \
src/fortran/minpack/fdjac2.f \
src/fortran/minpack/r1mpyq.f


GATEWAY_C_SOURCES = sci_gateway/c/sci_semidef.c \
sci_gateway/c/sci_qld.c \
sci_gateway/c/gw_optimization.c \
sci_gateway/c/sci_optim.c \
sci_gateway/c/sci_fsolv.c \
sci_gateway/c/sci_sqrsolve.c \
sci_gateway/c/sci_qp_solve.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_semidef.f \
sci_gateway/fortran/sci_f_fsolve.f \
sci_gateway/fortran/sci_f_optim.f \
sci_gateway/fortran/boptim.f \
sci_gateway/fortran/bsolv.f \
sci_gateway/fortran/bjlsqrsolv.f \
sci_gateway/fortran/blsqrsolv.f \
sci_gateway/fortran/bjsolv.f \
sci_gateway/fortran/intlsqrsolve.f \
sci_gateway/fortran/Ex-optim.f \
sci_gateway/fortran/Ex-fsolve.f \
sci_gateway/fortran/Ex-lsqrsolve.f

libscioptimization_la_CFLAGS=  	-I$(srcdir)/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/dynamic_link/includes

pkglib_LTLIBRARIES = libscioptimization.la

libscioptimization_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscioptimization_la_SOURCES = $(OPTIMIZATION_C_SOURCES) $(OPTIMIZATION_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(MINPACK_SOURCES)

libscioptimization_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la  $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/dynamic_link/libscidynamic_link.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 


#### Target ######
modulename=optimization


#### optimization : Conf files ####
libscioptimization_la_rootdir = $(mydatadir)
libscioptimization_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### m2sci : Macros ####
MACROSDIRSEXT =  macros/neldermead \
	macros/optimbase \
	macros/optimsimplex

####optimization : init scripts ####
libscioptimization_la_etcdir = $(mydatadir)/etc
libscioptimization_la_etc_DATA = etc/optimization.quit etc/optimization.start

####optimization : gateway declaration ####
libscioptimization_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscioptimization_la_sci_gateway_DATA = sci_gateway/optimization_gateway.xml


include $(top_srcdir)/Makefile.incl.am
