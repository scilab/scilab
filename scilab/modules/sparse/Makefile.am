# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
    src/fortran/isort1.f \
    src/fortran/spt.f \
    src/fortran/sz2ptr.f \
    src/fortran/spreshape.f \
    src/fortran/ordmmd.f \
	src/fortran/blkfc1.f \
	src/fortran/blkfct.f \
	src/fortran/inpnv.f \
	src/fortran/symfct.f
	
GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sparse_gw.cpp \
    sci_gateway/cpp/sci_adj2sp.cpp \
    sci_gateway/cpp/sci_full.cpp \
    sci_gateway/cpp/sci_nnz.cpp \
    sci_gateway/cpp/sci_sp2adj.cpp \
    sci_gateway/cpp/sci_sparse.cpp \
    sci_gateway/cpp/sci_spcompack.cpp \
    sci_gateway/cpp/sci_spget.cpp \
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
	sci_gateway/cpp/sci_blkfc1i.cpp
	
libscisparse_la_CPPFLAGS = \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
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



libscisparse_algo_la_SOURCES = $(SPARSE_C_SOURCES) $(SPARSE_FORTRAN_SOURCES)
libscisparse_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libscisparse_algo_la_CPPFLAGS = $(libscisparse_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC = $(SPARSE_C_SOURCES)
INCLUDE_FLAGS = $(libscisparse_la_CPPFLAGS)

libscisparse_la_LIBADD = libscisparse-algo.la

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
