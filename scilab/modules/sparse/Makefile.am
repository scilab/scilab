# Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

SPARSE_C_SOURCES = \
    src/c/spUtils.c \
    src/c/lu.c \
    src/c/spFactor.c \
    src/c/spBuild.c \
    src/c/spOutput.c \
    src/c/spAllocate.c \
    src/c/spSolve.c

SPARSE_FORTRAN_SOURCES = \
    src/fortran/spt.f \
    src/fortran/sz2ptr.f \
    src/fortran/ordmmd.f \
	src/fortran/blkfc1.f \
	src/fortran/blkfct.f \
	src/fortran/blkslv.f \
	src/fortran/inpnv.f \
	src/fortran/symfct.f

COLPACK_CPP_SOURCES = \
    src/ColPack/src/Utilities/CoutLock.cpp \
    src/ColPack/src/Utilities/DisjointSets.cpp \
    src/ColPack/src/Utilities/File.cpp \
    src/ColPack/src/Utilities/MatrixDeallocation.cpp \
    src/ColPack/src/Utilities/Pause.cpp \
    src/ColPack/src/Utilities/StringTokenizer.cpp \
    src/ColPack/src/Utilities/Timer.cpp \
    src/ColPack/src/Utilities/command_line_parameter_processor.cpp \
    src/ColPack/src/Utilities/current_time.cpp \
    src/ColPack/src/Utilities/extra.cpp \
    src/ColPack/src/Utilities/mmio.cpp \
    src/ColPack/src/Utilities/stat.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphBicoloring.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphBicoloringInterface.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphCore.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphInputOutput.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphOrdering.cpp \
    src/ColPack/src/BipartiteGraphBicoloring/BipartiteGraphVertexCover.cpp \
    src/ColPack/src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoring.cpp \
    src/ColPack/src/BipartiteGraphPartialColoring/BipartiteGraphPartialColoringInterface.cpp \
    src/ColPack/src/BipartiteGraphPartialColoring/BipartiteGraphPartialOrdering.cpp \
    src/ColPack/src/GeneralGraphColoring/GraphColoring.cpp \
    src/ColPack/src/GeneralGraphColoring/GraphColoringInterface.cpp \
    src/ColPack/src/GeneralGraphColoring/GraphCore.cpp \
    src/ColPack/src/GeneralGraphColoring/GraphInputOutput.cpp \
    src/ColPack/src/GeneralGraphColoring/GraphOrdering.cpp \
    src/ColPack/src/Recovery/HessianRecovery.cpp \
    src/ColPack/src/Recovery/JacobianRecovery1D.cpp \
    src/ColPack/src/Recovery/JacobianRecovery2D.cpp \
    src/ColPack/src/Recovery/RecoveryCore.cpp \
    src/cpp/spCompGeneric.cpp \
    src/cpp/spCompJacobian.cpp \
    src/cpp/spCompHessian.cpp

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sparse_gw.cpp \
    sci_gateway/cpp/sci_adj2sp.cpp \
    sci_gateway/cpp/sci_full.cpp \
    sci_gateway/cpp/sci_nnz.cpp \
    sci_gateway/cpp/sci_sp2adj.cpp \
    sci_gateway/cpp/sci_sparse.cpp \
    sci_gateway/cpp/sci_spcompack.cpp \
    sci_gateway/cpp/sci_spget.cpp \
    sci_gateway/cpp/sci_spset.cpp \
    sci_gateway/cpp/sci_spzeros.cpp \
    sci_gateway/cpp/sci_spones.cpp \
    sci_gateway/cpp/sci_lufact.cpp \
    sci_gateway/cpp/sci_lusolve.cpp \
    sci_gateway/cpp/sci_luget.cpp \
    sci_gateway/cpp/sci_ludel.cpp \
    sci_gateway/cpp/sci_ordmmd.cpp \
    sci_gateway/cpp/sci_spchol.cpp \
    sci_gateway/cpp/sci_sfinit.cpp \
    sci_gateway/cpp/sci_symfcti.cpp \
    sci_gateway/cpp/sci_bfinit.cpp \
    sci_gateway/cpp/sci_inpnv.cpp \
    sci_gateway/cpp/sci_blkfc1i.cpp \
    sci_gateway/cpp/sci_blkslvi.cpp \
    sci_gateway/cpp/sci_percent_spCompGeneric_clear.cpp \
    sci_gateway/cpp/sci_percent_spCompGeneric_e.cpp \
    sci_gateway/cpp/sci_spCompHessian.cpp \
    sci_gateway/cpp/sci_spCompJacobian.cpp
	
libscisparse_la_CPPFLAGS = \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/ColPack/include/ \
    -I$(srcdir)/src/ColPack/src/Utilities/ \
    -I$(srcdir)/src/ColPack/src/Recovery/ \
    -I$(srcdir)/src/ColPack/src/GeneralGraphColoring \
    -I$(srcdir)/src/ColPack/src/BipartiteGraphPartialColoring \
    -I$(srcdir)/src/ColPack/src/BipartiteGraphBicoloring \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(EIGEN_CPPFLAGS) \
    $(AM_CPPFLAGS)

#-Isrc/c/
if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscisparse.la
noinst_LTLIBRARIES = libscisparse-algo.la
else
noinst_LTLIBRARIES = libscisparse-algo.la libscisparse.la
endif

if IS_MACOSX
#enforce -O0 for some files due to macOS crash
src/fortran/blkfct.lo: src/fortran/blkfct.f
	$(LIBTOOL) $(AM_V_lt)  --tag=F77 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(F77) $(AM_FFLAGS) $(FFLAGS) -O0 -c -o src/fortran/blkfct.lo src/fortran/blkfct.f

src/fortran/symfct.lo: src/fortran/symfct.f
	$(LIBTOOL) $(AM_V_lt)  --tag=F77 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(F77) $(AM_FFLAGS) $(FFLAGS) -O0 -c -o src/fortran/symfct.lo src/fortran/symfct.f

src/fortran/ordmmd.lo: src/fortran/ordmmd.f
	$(LIBTOOL) $(AM_V_lt)  --tag=F77 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(F77) $(AM_FFLAGS) $(FFLAGS) -O0 -c -o src/fortran/ordmmd.lo src/fortran/ordmmd.f
endif

libscisparse_algo_la_SOURCES = $(SPARSE_C_SOURCES) $(SPARSE_FORTRAN_SOURCES) $(COLPACK_CPP_SOURCES)
libscisparse_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libscisparse_algo_la_CPPFLAGS = $(libscisparse_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC = $(SPARSE_C_SOURCES)
INCLUDE_FLAGS = $(libscisparse_la_CPPFLAGS)

libscisparse_la_LIBADD = libscisparse-algo.la $(FLIBS)

#### Target ######
modulename=sparse


#### sparse : Conf files ####
libscisparse_la_rootdir = $(mydatadir)
libscisparse_la_root_DATA =  license.txt


#### sparse : init scripts ####
libscisparse_la_etcdir = $(mydatadir)/etc
libscisparse_la_etc_DATA = \
    etc/sparse.quit \
    etc/sparse.start

#### sparse : gateway declaration ####
libscisparse_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisparse_la_sci_gateway_DATA =

include $(top_srcdir)/Makefile.incl.am
