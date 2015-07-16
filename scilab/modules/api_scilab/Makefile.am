# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - Digiteo - Allan CORNET
#
# This file is distributed under the same license as the Scilab package.

### SOURCES ###

API_SCILAB_CXX_SOURCES = \
    src/cpp/api_boolean.cpp \
    src/cpp/api_boolean_sparse.cpp \
    src/cpp/api_common.cpp \
    src/cpp/api_double.cpp \
    src/cpp/api_int.cpp \
    src/cpp/api_list.cpp \
    src/cpp/api_poly.cpp \
    src/cpp/api_sparse.cpp \
    src/cpp/api_string.cpp \
    src/cpp/api_pointer.cpp \
    src/cpp/api_handle.cpp \
    src/cpp/api_error.cpp \
    src/cpp/api_hypermat.cpp \
    src/cpp/returnProperty.cpp \
    src/cpp/returnPropertyList.cpp \
    src/cpp/api_optional.cpp \
    src/cpp/api_boolean_cpp.cpp \
    src/cpp/api_common_cpp.cpp \
    src/cpp/api_double_cpp.cpp \
    src/cpp/api_int_cpp.cpp \
    src/cpp/api_list_cpp.cpp \
    src/cpp/api_sparse_cpp.cpp \
    src/cpp/api_string_cpp.cpp \
    src/cpp/api_pointer_cpp.cpp \
    src/cpp/api_handle_cpp.cpp

libsciapi_scilab_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/call_scilab/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/elementary_functions/includes \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/threads/includes \
    $(AM_CPPFLAGS)

if MAINTAINER_MODE
pkglib_LTLIBRARIES = libsciapi_scilab.la
else
noinst_LTLIBRARIES = libsciapi_scilab.la
endif


libsciapi_scilab_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libsciapi_scilab_la_SOURCES = $(API_SCILAB_C_SOURCES) $(API_SCILAB_CXX_SOURCES)

# For the code check (splint)
CHECK_SRC= $(libsciapi_scilab_la_SOURCES)
INCLUDE_FLAGS = $(libsciapi_scilab_la_CPPFLAGS)

libsciapi_scilab_la_LIBADD =
#


#### Target ######
modulename=api_scilab


#### api_scilab : Conf files ####
libsciapi_scilab_la_rootdir = $(mydatadir)
libsciapi_scilab_la_root_DATA =  license.txt


#### api_scilab : init scripts ####
libsciapi_scilab_la_etcdir = $(mydatadir)/etc
libsciapi_scilab_la_etc_DATA = \
    etc/api_scilab.quit \
    etc/api_scilab.start

#### api_scilab : include files ####
libsciapi_scilab_la_includedir=$(pkgincludedir)
libsciapi_scilab_la_include_HEADERS = \
includes/api_boolean.h \
includes/api_boolean_sparse.h \
includes/api_common.h \
includes/api_error.h \
includes/api_double.h \
includes/api_int.h \
includes/api_list.h \
includes/api_pointer.h \
includes/api_poly.h \
includes/api_sparse.h \
includes/api_string.h \
includes/api_handle.h \
includes/api_optional.h \
includes/api_scilab.h \
includes/dynlib_api_scilab.h \
includes/api_boolean.hxx \
includes/api_common.hxx \
includes/api_double.hxx \
includes/api_handle.hxx \
includes/api_int.hxx \
includes/api_list.hxx \
includes/api_pointer.hxx \
includes/api_scilab.hxx \
includes/api_sparse.hxx \
includes/api_string.hxx \
includes/api_variable.hxx

include $(top_srcdir)/Makefile.incl.am
