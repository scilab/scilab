# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
# Copyright (C) 2011 - DIGITEO - Allan CORNET
#
# This file is distributed under the same license as the Scilab package.

SPECIAL_FUNCTIONS_C_SOURCES = src/c/zbeshv.c

# List of the cpp files
SPECIAL_FUNCTIONS_CXX_SOURCES = \
    src/cpp/faddeeva.cpp

SPECIAL_FUNCTIONS_FORTRAN_SOURCES = \
    src/fortran/dbesig.f \
    src/fortran/dbesjg.f \
    src/fortran/dbeskg.f \
    src/fortran/dbesyg.f \
    src/fortran/psi.f \
    src/fortran/zbesig.f \
    src/fortran/zbesjg.f \
    src/fortran/zbeskg.f \
    src/fortran/zbesyg.f

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_bessely.c \
    sci_gateway/c/sci_beta.c \
    sci_gateway/c/sci_legendre.c \
    sci_gateway/c/sci_besselh.c \
    sci_gateway/c/sci_besseli.c \
    sci_gateway/c/sci_besselj.c \
    sci_gateway/c/sci_besselk.c

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_faddeeva.cpp \
    sci_gateway/cpp/sci_gamma.cpp \
    sci_gateway/cpp/sci_lgamma.cpp \
    sci_gateway/cpp/sci_dlgamma.cpp

# Includes need for the compilation
libscispecial_functions_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/sci_gateway/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/core/src/c/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    $(AM_CPPFLAGS)


pkglib_LTLIBRARIES = libscispecial_functions.la
noinst_LTLIBRARIES = libscispecial_functions-algo.la


libscispecial_functions_algo_la_SOURCES = $(SPECIAL_FUNCTIONS_C_SOURCES) $(SPECIAL_FUNCTIONS_FORTRAN_SOURCES) $(SPECIAL_FUNCTIONS_CXX_SOURCES)
libscispecial_functions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)

libscispecial_functions_algo_la_CPPFLAGS = $(libscispecial_functions_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(SPECIAL_FUNCTIONS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscispecial_functions_la_CPPFLAGS)

libscispecial_functions_la_LIBADD = libscispecial_functions-algo.la

#### Target ######
modulename=special_functions


#### special_functions : Conf files ####
libscispecial_functions_la_rootdir = $(mydatadir)
libscispecial_functions_la_root_DATA =  license.txt



#### special_functions : init scripts ####
libscispecial_functions_la_etcdir = $(mydatadir)/etc
libscispecial_functions_la_etc_DATA = etc/special_functions.quit etc/special_functions.start

#### special_functions : gateway declaration ####
libscispecial_functions_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscispecial_functions_la_sci_gateway_DATA = sci_gateway/special_functions_gateway.xml


include $(top_srcdir)/Makefile.incl.am
