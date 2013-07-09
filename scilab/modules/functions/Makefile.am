# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO - Allan CORNET
#
# This file is distributed under the same license as the Scilab package.

GATEWAY_C_SOURCES = \
sci_gateway/c/gw_functions.c

GATEWAY_CPP_SOURCES =  \
sci_gateway/cpp/sci_exec.cpp \
sci_gateway/cpp/sci_functionlist.cpp \
sci_gateway/cpp/sci_whereis.cpp \
sci_gateway/cpp/sci_execstr.cpp \
sci_gateway/cpp/sci_getCurrentThreadId.cpp \
sci_gateway/cpp/sci_getThreads.cpp \
sci_gateway/cpp/sci_macrovar.cpp

libscifunctions_la_CPPFLAGS = \
-I$(srcdir)/includes/ \
-I$(srcdir)/src/c/ \
-I$(top_srcdir)/modules/functions_manager/includes/ \
-I$(top_srcdir)/modules/symbol/includes/ \
-I$(top_srcdir)/modules/ast/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes/ \
-I$(top_srcdir)/modules/operations/includes/ \
-I$(top_srcdir)/modules/localization/includes/ \
-I$(top_srcdir)/modules/parse/includes/ \
-I$(top_srcdir)/modules/string/includes/ \
-I$(top_srcdir)/modules/fileio/includes \
-I$(top_srcdir)/modules/system_env/includes \
-I$(top_srcdir)/modules/console/includes \
-I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/types/includes/ \
-I$(top_srcdir)/modules/threads/includes/ \
-I$(top_srcdir)/modules/elementary_functions/includes/ \
-I$(top_srcdir)/modules/dynamic_link/includes/ \
$(EIGEN_CPPFLAGS) \
$(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscifunctions.la

libscifunctions_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(FUNCTIONS_SOURCES) $(GATEWAY_SOURCES)
INCLUDE_FLAGS = $(libscifunctions_la_CPPFLAGS)

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
