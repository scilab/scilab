# Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2017 - ESI-Group - Cedric Delamarre
#
# This file is distributed under the same license as the Scilab package.


WEBTOOLS_C_SOURCES = \
    src/c/DllmainWebtools.c

WEBTOOLS_CPP_SOURCES = \
    src/cpp/webtools.cpp \
    src/cpp/json.cpp \
    src/cpp/fromJSON.cpp \
    src/cpp/toJSON.cpp \
    src/cpp/url_tools.cpp \
    src/cpp/sciCurl.cpp \
    src/cpp/json_rpc.cpp \
    src/cpp/evalVisitor.cpp \
    src/cpp/lsp_dap_tools.cpp \
    src/cpp/documentAnalysisVisitor.cpp \
    src/cpp/formattingVisitor.cpp \
    src/cpp/lsp_interface.cpp \
    src/cpp/dap_interface.cpp \
    src/cpp/lspEndpoint.cpp \
    src/cpp/runtimeSnapshot.cpp \
    src/cpp/lspDocumentSync.cpp \
    src/cpp/lspDiagnostics.cpp \
    src/cpp/lspSemanticTokens.cpp \
    src/cpp/lspCompletionHelp.cpp \
    src/cpp/lspNavigation.cpp \
    src/cpp/lspProtocolUtils.cpp \
    src/cpp/lspFormatting.cpp \
    src/cpp/dapEndpoint.cpp \
    src/cpp/windows_tools.cpp

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_http_get.cpp \
    sci_gateway/cpp/sci_http_put.cpp \
    sci_gateway/cpp/sci_http_post.cpp \
    sci_gateway/cpp/sci_http_patch.cpp \
    sci_gateway/cpp/sci_http_put_post.cpp \
    sci_gateway/cpp/sci_http_delete.cpp \
    sci_gateway/cpp/sci_http_upload.cpp \
    sci_gateway/cpp/sci_url_encode.cpp \
    sci_gateway/cpp/sci_url_decode.cpp \
    sci_gateway/cpp/sci_url_split.cpp \
    sci_gateway/cpp/sci_toJSON.cpp \
    sci_gateway/cpp/sci_fromJSON.cpp \
    sci_gateway/cpp/gateway_tools.cpp \
    sci_gateway/cpp/webtools_gw.cpp

libsciwebtools_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/sci_gateway/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/completion/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/functions_manager/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/history_manager/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/slint/includes/ \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/preferences/includes \
    $(CPPSERVER_CFLAGS) \
    $(CURL_CFLAGS) \
    $(PCRE2_CFLAGS) \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

libsciwebtools_algo_la_CPPFLAGS = $(libsciwebtools_la_CPPFLAGS) -Wno-unused-parameter

pkglib_LTLIBRARIES = libsciwebtools.la
noinst_LTLIBRARIES = libsciwebtools-algo.la

libsciwebtools_algo_la_LDFLAGS = $(CURL_LIBS) $(CPPSERVER_LIBS)
libsciwebtools_algo_la_SOURCES = $(WEBTOOLS_CPP_SOURCES) $(WEBTOOLS_C_SOURCES)

libsciwebtools_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libsciwebtools_la_LIBADD = libsciwebtools-algo.la $(CPPSERVER_LIBS)

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
libsciwebtools_la_etc_DATA = \
etc/webtools.quit \
etc/webtools.start \
etc/help_md.xsl \
etc/help_proto.xsl


#### webtools : include files ####
libsciwebtools_la_includedir=$(pkgincludedir)
libsciwebtools_la_include_HEADERS = \
includes/json.hxx \
includes/lsp_interface.h \
includes/dap_interface.h \
includes/dynlib_webtools.h \
includes/dynlib_webtools_gw.h

EXTRA_DIST = \
src/cpp/dapEndpoint.hxx \
src/cpp/evalVisitor.hxx \
src/cpp/json_rpc.hxx \
src/cpp/lspEndpoint.hxx \
src/cpp/lspProtocolUtils.hxx \
src/cpp/lspDiagnostics.hxx \
src/cpp/lsp_dap_tools.hxx \
src/cpp/nlohmann_json.hxx \
src/cpp/documentAnalysisVisitor.hxx \
src/cpp/formattingVisitor.hxx \
src/cpp/nlohmann/json.hpp

include $(top_srcdir)/Makefile.incl.am
