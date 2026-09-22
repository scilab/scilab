# Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

POLYNOMIALS_FORTRAN_SOURCES = \
    src/fortran/residu.f \
    src/fortran/dpmul1.f \
    src/fortran/wprxc.f \
    src/fortran/recbez.f \
    src/fortran/sfact1.f \
    src/fortran/dpodiv.f \
    src/fortran/dpsimp.f \
    src/fortran/bezstp.f \
    src/fortran/dprxc.f \
    src/fortran/wpmul1.f \
    src/fortran/horner.f \
    src/fortran/idegre.f \
    src/fortran/wpodiv.f \
    src/fortran/dpmul.f \
    src/fortran/sfact2.f \
    src/fortran/blktit.f

POLYNOMIALS_CXX_SOURCES = \
    src/cpp/rpoly.cpp \
    src/cpp/find_polynomial_roots_jenkins_traub.cc \
    src/cpp/polynomial.cc


GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/polynomials_gw.cpp \
    sci_gateway/cpp/sci_poly.cpp \
    sci_gateway/cpp/sci_varn.cpp \
    sci_gateway/cpp/sci_roots.cpp \
    sci_gateway/cpp/sci_degree.cpp \
    sci_gateway/cpp/sci_coeff.cpp \
    sci_gateway/cpp/sci_pppdiv.cpp \
    sci_gateway/cpp/sci_simpMode.cpp \
    sci_gateway/cpp/sci_polyDisplay.cpp \
    sci_gateway/cpp/sci_simp.cpp \
    sci_gateway/cpp/sci_sfact.cpp \
    sci_gateway/cpp/sci_bezout.cpp


libscipolynomials_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/console/includes \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(EIGEN_CPPFLAGS) \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libscipolynomials.la
noinst_LTLIBRARIES = libscipolynomials-algo.la
else
noinst_LTLIBRARIES = libscipolynomials-algo.la libscipolynomials.la
endif



libscipolynomials_algo_la_SOURCES = $(POLYNOMIALS_FORTRAN_SOURCES) $(POLYNOMIALS_CXX_SOURCES)
libscipolynomials_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscipolynomials_algo_la_CPPFLAGS = $(libscipolynomials_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= ) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscipolynomials_la_CPPFLAGS)

libscipolynomials_la_LIBADD = libscipolynomials-algo.la $(FLIBS)

#### Target ######
modulename=polynomials


#### polynomials : Conf files ####
libscipolynomials_la_rootdir = $(mydatadir)
libscipolynomials_la_root_DATA =  license.txt


#### polynomials : init scripts ####
libscipolynomials_la_etcdir = $(mydatadir)/etc
libscipolynomials_la_etc_DATA = etc/polynomials.quit etc/polynomials.start

include $(top_srcdir)/Makefile.incl.am
