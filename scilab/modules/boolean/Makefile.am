# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.



BOOLEAN_CPP_SOURCES = \
    src/cpp/boolean_gw.cpp

BOOLEAN_C_SOURCES = \
    src/c/vect_or.c \
    src/c/vect_and.c

BOOLEAN_FORTRAN_SOURCES = src/fortran/logic.f


GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_bool2s.cpp \
    sci_gateway/cpp/sci_find.cpp

GATEWAY_C_SOURCES = sci_gateway/c/sci_and.c \
    sci_gateway/c/sci_or.c

GATEWAY_FORTRAN_SOURCES =

libsciboolean_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/functions_manager/includes \
    -I$(top_srcdir)/modules/types/includes \
    -I$(top_srcdir)/modules/symbol/includes \
    -I$(top_srcdir)/modules/ast/includes \
    -I$(top_srcdir)/modules/elementary_functions/includes \
    -I$(top_srcdir)/modules/console/includes \
    -I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/operations/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/system_env/includes \
    -I$(top_srcdir)/modules/threads/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/string/includes \
    $(EIGEN_CPPFLAGS) \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciboolean.la
noinst_LTLIBRARIES = libsciboolean-algo.la
else
noinst_LTLIBRARIES = libsciboolean-algo.la libsciboolean.la
endif



libsciboolean_algo_la_SOURCES = $(BOOLEAN_C_SOURCES) $(BOOLEAN_FORTRAN_SOURCES) $(BOOLEAN_CPP_SOURCES) $(GATEWAY_CPP_SOURCES)
libsciboolean_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libsciboolean_algo_la_CPPFLAGS = $(libsciboolean_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(BOOLEAN_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciboolean_la_CPPFLAGS)

libsciboolean_la_LIBADD = libsciboolean-algo.la

modulename=boolean

#### boolean : Conf files ####
libsciboolean_la_rootdir = $(mydatadir)
libsciboolean_la_root_DATA =  license.txt


#### boolean : init scripts ####
libsciboolean_la_etcdir = $(mydatadir)/etc
libsciboolean_la_etc_DATA = etc/boolean.quit etc/boolean.start

#### boolean : gateway declaration ####
libsciboolean_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciboolean_la_sci_gateway_DATA = sci_gateway/boolean_gateway.xml

include $(top_srcdir)/Makefile.incl.am
