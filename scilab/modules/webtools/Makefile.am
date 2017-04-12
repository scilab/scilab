# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2017 - ESI-Group - Cedric Delamarre
#
# This file is distributed under the same license as the Scilab package.


WEBTOOLS_C_SOURCES = \
    src/c/DllmainWebtools.c \
    src/c/jsmn.c

WEBTOOLS_CPP_SOURCES = \
    src/cpp/sciCurl.cpp \
    src/cpp/webtools.cpp \
    src/cpp/json.cpp

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_http_get.cpp \
    sci_gateway/cpp/sci_http_put.cpp \
    sci_gateway/cpp/sci_http_post.cpp \
    sci_gateway/cpp/sci_http_patch.cpp \
    sci_gateway/cpp/sci_http_put_post.cpp \
    sci_gateway/cpp/sci_http_delete.cpp \
    sci_gateway/cpp/sci_http_upload.cpp \
    sci_gateway/cpp/sci_toJSON.cpp \
    sci_gateway/cpp/sci_fromJSON.cpp \
    sci_gateway/cpp/checkCommonOpt.cpp

libsciwebtools_la_CPPFLAGS= \
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
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/preferences/includes \
    $(CURL_CFLAGS) \
    $(AM_CPPFLAGS)

libsciwebtools_algo_la_CPPFLAGS = $(libsciwebtools_la_CPPFLAGS)

pkglib_LTLIBRARIES = libsciwebtools.la
noinst_LTLIBRARIES = libsciwebtools-algo.la

libsciwebtools_algo_la_LDFLAGS = $(CURL_LIBS)
libsciwebtools_algo_la_SOURCES = $(WEBTOOLS_CPP_SOURCES) $(WEBTOOLS_C_SOURCES)

libsciwebtools_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libsciwebtools_la_LIBADD = libsciwebtools-algo.la

# For the code check (splint)
CHECK_SRC= $(libsciwebtools_la_SOURCES)
INCLUDE_FLAGS = $(libsciwebtools_la_CPPFLAGS)


#### Target ######
modulename=webtools


#### webtools : Conf files ####
libsciwebtools_la_rootdir = $(mydatadir)
libsciwebtools_la_root_DATA =  license.txt


#### webtools : init scripts ####
libsciwebtools_la_etcdir = $(mydatadir)/etc
libsciwebtools_la_etc_DATA = etc/webtools.quit etc/webtools.start

#### webtools : gateway declaration ####
libsciwebtools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciwebtools_la_sci_gateway_DATA = sci_gateway/webtools_gateway.xml

#### webtools : include files ####
libsciwebtools_la_includedir=$(pkgincludedir)

include $(top_srcdir)/Makefile.incl.am
