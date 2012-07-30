# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Allan CORNET
#
# This file is distributed under the same license as the Scilab package.

FUNCTIONS_C_SOURCES = src/c/getmacroslist.c \
	src/c/librarieslist.c \
	src/c/libraryinfo.c \
	src/c/isScilabFunction.c \
	src/c/searchmacroinlibraries.c

FUNCTIONS_FORTRAN_SOURCES = src/fortran/whereis.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_functions.c \
sci_gateway/c/sci_lib.c \
sci_gateway/c/sci_deff.c \
sci_gateway/c/sci_execstr.c \
sci_gateway/c/sci_exec.c \
sci_gateway/c/sci_whereis.c \
sci_gateway/c/sci_librarieslist.c \
sci_gateway/c/sci_libraryinfo.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/intlib.f \
sci_gateway/fortran/intdeff.f \
sci_gateway/fortran/intexecstr.f \
sci_gateway/fortran/intexec.f

libscifunctions_la_CPPFLAGS = -I$(srcdir)/includes/ \
				-I$(srcdir)/src/c/ \
				-I$(top_srcdir)/libs/MALLOC/includes/ \
				-I$(top_srcdir)/modules/core/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes/ \
				-I$(top_srcdir)/modules/fileio/includes/ \
				-I$(top_srcdir)/modules/call_scilab/includes/ \
				-I$(top_srcdir)/modules/io/includes/ \
				-I$(top_srcdir)/modules/string/includes/ \
				-I$(top_srcdir)/modules/localization/includes/ \
				$(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscifunctions.la
noinst_LTLIBRARIES = libscifunctions-algo.la


libscifunctions_algo_la_SOURCES = $(FUNCTIONS_C_SOURCES) $(FUNCTIONS_FORTRAN_SOURCES)
libscifunctions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libscifunctions_algo_la_CPPFLAGS = $(libscifunctions_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(FUNCTIONS_SOURCES) $(GATEWAY_SOURCES)
INCLUDE_FLAGS = $(libscifunctions_la_CPPFLAGS)

libscifunctions_la_LIBADD =   \
	libscifunctions-algo.la

#### Target ######
modulename=functions


#### functions : Conf files ####
libscifunctions_la_rootdir = $(mydatadir)
libscifunctions_la_root_DATA =  license.txt 


#### functions : init scripts ####
libscifunctions_la_etcdir = $(mydatadir)/etc
libscifunctions_la_etc_DATA = etc/functions.quit etc/functions.start

#### functions : gateway declaration ####
libscifunctions_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifunctions_la_sci_gateway_DATA = sci_gateway/functions_gateway.xml

include $(top_srcdir)/Makefile.incl.am
