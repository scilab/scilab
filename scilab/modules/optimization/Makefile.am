# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

OPTIMIZATION_C_SOURCES = \
    src/c/sp.c

OPTIMIZATION_FORTRAN_SOURCES = \
    src/fortran/gcbd.f \
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
    src/fortran/n1fc1.f \
    src/fortran/ffinf1.f \
    src/fortran/ctcab.f \
    src/fortran/n1gc2b.f \
    src/fortran/fprf2.f \
    src/fortran/icse.f \
    src/fortran/rlbd.f \
    src/fortran/calmaj.f \
    src/fortran/calbx.f \
    src/fortran/rednor.f \
    src/fortran/zgcbd.f \
    src/fortran/writebuf.f \
    src/fortran/qpgen1sci.f \
    src/fortran/rdmps1.f \
    src/fortran/rdmpsz.f

MINPACK_SOURCES = \
    src/fortran/minpack/lmdif.f \
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

OPTIMIZATION_CPP_SOURCES = \
    src/cpp/optimizationfunctions.cpp \
    src/cpp/checkoptimerror.cpp \
    src/cpp/scioptimfunctions.cpp

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_optim.cpp \
    sci_gateway/cpp/sci_fsolve.cpp \
    sci_gateway/cpp/sci_semidef.cpp \
    sci_gateway/cpp/sci_lsqrsolve.cpp \
    sci_gateway/cpp/sci_readmps.cpp

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_qld.c \
    sci_gateway/c/sci_qp_solve.c

GATEWAY_FORTRAN_SOURCES = \
    sci_gateway/fortran/Ex-optim.f \
    sci_gateway/fortran/Ex-fsolve.f \
    sci_gateway/fortran/Ex-lsqrsolve.f

libscioptimization_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(AM_CPPFLAGS)


pkglib_LTLIBRARIES = libscioptimization.la
noinst_LTLIBRARIES = libscioptimization-algo.la


libscioptimization_algo_la_SOURCES = $(OPTIMIZATION_C_SOURCES) $(OPTIMIZATION_CPP_SOURCES) $(OPTIMIZATION_FORTRAN_SOURCES) $(MINPACK_SOURCES)
libscioptimization_la_SOURCES = $(GATEWAY_CPP_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscioptimization_algo_la_CPPFLAGS = $(libscioptimization_la_CPPFLAGS)

libscioptimization_la_LIBADD = libscioptimization-algo.la


#### Target ######
modulename=optimization


#### optimization : Conf files ####
libscioptimization_la_rootdir = $(mydatadir)
libscioptimization_la_root_DATA =  license.txt

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
