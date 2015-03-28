# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

MEXLIB_CPP_SOURCES = \
    src/cpp/mexlib.cpp \
    src/cpp/sci_gateway.cpp


MEXLIB_FLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
	-I$(top_srcdir)/modules/localization/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	-I$(top_srcdir)/modules/call_scilab/includes/ \
	-I$(top_srcdir)/modules/elementary_functions/includes/ \
	-I$(top_srcdir)/modules/string/includes \
	-I$(top_srcdir)/modules/threads/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libmat.la libmex.la libmx.la
else
noinst_LTLIBRARIES = libmat.la libmex.la libmx.la
endif


libmex_la_SOURCES = $(MEXLIB_CPP_SOURCES)

libmex_la_LIBADD = $(MEXLIB_DEP)

libmex_la_CPPFLAGS =  \
    $(MEXLIB_FLAGS) \
	$(AM_CPPFLAGS)

libmat_la_SOURCES = $(MEXLIB_CPP_SOURCES)

libmat_la_LIBADD = $(MEXLIB_DEP)

libmat_la_CPPFLAGS = \
    $(MEXLIB_FLAGS) \
    $(AM_CPPFLAGS)

libmat_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libmx_la_SOURCES = $(MEXLIB_CPP_SOURCES)

libmx_la_LIBADD = $(MEXLIB_DEP)

libmx_la_CPPFLAGS = \
    $(MEXLIB_FLAGS) \
	$(AM_CPPFLAGS)

libmx_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

#### Target ######
modulename=mexlib

# For the code check (splint)
CHECK_SRC= $(MEXLIB_CPP_SOURCES)
INCLUDE_FLAGS = $(MEXLIB_FLAGS) $(AM_CPPFLAGS)

#### mexlib : Conf files ####
libmex_la_rootdir = $(mydatadir)
libmex_la_root_DATA =  license.txt

#### mexlib : init scripts ####
libmex_la_etcdir = $(mydatadir)/etc
libmex_la_etc_DATA = \
    etc/mexlib.quit \
    etc/mexlib.start


#### mexlib : include files ####
libmex_la_includedir=$(pkgincludedir)
libmex_la_include_HEADERS = \
includes/mex.h \
includes/sci_gateway.h

include $(top_srcdir)/Makefile.incl.am
