# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.


BOOLEAN_C_SOURCES = src/c/matboolean.c \
src/c/vect_or.c \
src/c/vect_and.c

BOOLEAN_FORTRAN_SOURCES = src/fortran/logic.f \
src/fortran/find.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_and.c \
sci_gateway/c/sci_or.c \
sci_gateway/c/sci_find.c \
sci_gateway/c/sci_bool2s.c \
sci_gateway/c/gw_boolean.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_bool2s.f \
sci_gateway/fortran/sci_f_find.f

libsciboolean_la_CPPFLAGS= -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				$(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciboolean.la
noinst_LTLIBRARIES = libsciboolean-algo.la
else
noinst_LTLIBRARIES = libsciboolean-algo.la libsciboolean.la
endif



libsciboolean_algo_la_SOURCES = $(BOOLEAN_C_SOURCES) $(BOOLEAN_FORTRAN_SOURCES)
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
