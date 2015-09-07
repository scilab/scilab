# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

LINEAR_ALGEBRA_CPP_SOURCES = \
    src/cpp/scischur.cpp \
    src/cpp/schurSelect.cpp

LINEAR_ALGEBRA_C_SOURCES = \
    src/c/schurtable.c \
    src/c/issymmetric.c \
    src/c/lu.c \
    src/c/qr.c \
    src/c/hess.c \
    src/c/eigen.c \
    src/c/balanc.c \
    src/c/chol.c \
    src/c/svd.c \
    src/c/rcond.c \
    src/c/det.c \
    src/c/lsq.c \
    src/c/norm.c

LINEAR_ALGEBRA_FORTRAN_SOURCES = \
    src/fortran/DGELSY1.f \
    src/fortran/ZGELSY1.f \
    src/fortran/find.f

GATEWAY_C_SOURCES = \
    sci_gateway/c/assembleEigenvectors.c \
    sci_gateway/c/sci_norm.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/linear_algebra_gw.cpp \
    sci_gateway/cpp/sci_chol.cpp \
    sci_gateway/cpp/sci_bdiag.cpp \
    sci_gateway/cpp/sci_inv.cpp \
    sci_gateway/cpp/sci_svd.cpp \
    sci_gateway/cpp/sci_rcond.cpp \
    sci_gateway/cpp/sci_qr.cpp \
    sci_gateway/cpp/sci_lu.cpp \
    sci_gateway/cpp/sci_lsq.cpp \
    sci_gateway/cpp/sci_hess.cpp \
    sci_gateway/cpp/sci_spec.cpp \
    sci_gateway/cpp/sci_det.cpp \
    sci_gateway/cpp/sci_balanc.cpp \
    sci_gateway/cpp/sci_schur.cpp

libscilinear_algebra_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/console/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/elementary_functions/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscilinear_algebra.la
noinst_LTLIBRARIES = libscilinear_algebra-algo.la
else
noinst_LTLIBRARIES = libscilinear_algebra-algo.la libscilinear_algebra.la
endif



libscilinear_algebra_algo_la_SOURCES = $(LINEAR_ALGEBRA_FORTRAN_SOURCES) $(LINEAR_ALGEBRA_C_SOURCES) $(LINEAR_ALGEBRA_CPP_SOURCES)
libscilinear_algebra_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)
libscilinear_algebra_algo_la_CPPFLAGS = $(libscilinear_algebra_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(LINEAR_ALGEBRA_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscilinear_algebra_la_CPPFLAGS)

libscilinear_algebra_la_LIBADD = libscilinear_algebra-algo.la

#### Target ######
modulename=linear_algebra


#### linear_algebra : Conf files ####
libscilinear_algebra_la_rootdir = $(mydatadir)
libscilinear_algebra_la_root_DATA =  license.txt


#### linear_algebra : init scripts ####
libscilinear_algebra_la_etcdir = $(mydatadir)/etc
libscilinear_algebra_la_etc_DATA = etc/linear_algebra.quit etc/linear_algebra.start
#### linear_algebra : gateway declaration ####
libscilinear_algebra_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscilinear_algebra_la_sci_gateway_DATA = sci_gateway/linear_algebra_gateway.xml


include $(top_srcdir)/Makefile.incl.am
