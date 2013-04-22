# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

SPARSE_C_SOURCES =

SPARSE_FORTRAN_SOURCES = \
src/fortran/isort1.f \
src/fortran/spt.f \
src/fortran/sz2ptr.f \
src/fortran/spreshape.f

GATEWAY_C_SOURCES =

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
sci_gateway/cpp/sci_spones.cpp


libscisparse_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/ast/includes/ \
				-I$(top_srcdir)/modules/types/includes/ \
				-I$(top_srcdir)/modules/symbol/includes/ \
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
libscisparse_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscisparse_algo_la_CPPFLAGS = $(libscisparse_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC = $(SPARSE_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscisparse_la_CPPFLAGS)

libscisparse_la_LIBADD = libscisparse-algo.la

#### Target ######
modulename=sparse


#### sparse : Conf files ####
libscisparse_la_rootdir = $(mydatadir)
libscisparse_la_root_DATA =  license.txt


#### sparse : init scripts ####
libscisparse_la_etcdir = $(mydatadir)/etc
libscisparse_la_etc_DATA = etc/sparse.quit etc/sparse.start

#### sparse : gateway declaration ####
libscisparse_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscisparse_la_sci_gateway_DATA =

include $(top_srcdir)/Makefile.incl.am
