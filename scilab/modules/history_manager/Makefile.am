# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007 - INRIA - Allan CORNET
#
# This file is distributed under the same license as the Scilab package.

# List of the cpp files
HISTORY_MANAGER_CPP_SOURCES = \
    src/cpp/HistoryFile.cpp \
    src/cpp/HistoryManager.cpp \
    src/cpp/HistorySearch.cpp

# List of the c files
HISTORY_MANAGER_C_SOURCES = src/c/getCommentDateSession.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/history_manager_gw.cpp \
    sci_gateway/cpp/sci_addhistory.cpp \
    sci_gateway/cpp/sci_displayhistory.cpp \
    sci_gateway/cpp/sci_gethistory.cpp \
    sci_gateway/cpp/sci_gethistoryfile.cpp \
    sci_gateway/cpp/sci_historysize.cpp \
    sci_gateway/cpp/sci_loadhistory.cpp \
    sci_gateway/cpp/sci_removelinehistory.cpp \
    sci_gateway/cpp/sci_resethistory.cpp \
    sci_gateway/cpp/sci_saveafterncommands.cpp \
    sci_gateway/cpp/sci_saveconsecutivecommands.cpp \
    sci_gateway/cpp/sci_savehistory.cpp \
    sci_gateway/cpp/sci_sethistoryfile.cpp \
    sci_gateway/cpp/sci_historymanager.cpp

if GUI

# List of the c files
HISTORY_MANAGER_C_SOURCES += src/jni/HistoryManagement_wrap.c

USEANT=1

GIWS_WRAPPERS =

if GIWS
BUILT_SOURCES=giws
endif

endif



# Includes need for the compilation
libscihistory_manager_la_CPPFLAGS = \
    $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/history_browser/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(AM_CPPFLAGS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscihistory_manager.la
noinst_LTLIBRARIES = libscihistory_manager-algo.la


# All the sources needed by libscihistory_manager.la
libscihistory_manager_algo_la_SOURCES = $(HISTORY_MANAGER_C_SOURCES) $(HISTORY_MANAGER_CPP_SOURCES)
libscihistory_manager_la_SOURCES = $(GATEWAY_CPP_SOURCES)
libscihistory_manager_algo_la_CPPFLAGS = $(libscihistory_manager_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(HISTORY_MANAGER_C_SOURCES)
INCLUDE_FLAGS = $(libscihistory_manager_la_CPPFLAGS)

## Libraries necessary
libscihistory_manager_la_LIBADD = libscihistory_manager-algo.la
modulename=history_manager

#### history_manager : Conf files ####
libscihistory_manager_la_rootdir = $(mydatadir)
libscihistory_manager_la_root_DATA =  license.txt

#### history_manager : init scripts ####
libscihistory_manager_la_etcdir = $(mydatadir)/etc
libscihistory_manager_la_etc_DATA = \
	etc/history_manager.quit \
	etc/history_manager.start \
	etc/XConfiguration-history.xml \
	etc/XConfiguration-history.xsl

#### history_manager : gateway declaration ####
libscihistory_manager_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihistory_manager_la_sci_gateway_DATA = sci_gateway/history_manager_gateway.xml

#### history_manager : include files ####
libscihistory_manager_la_includedir=$(pkgincludedir)
libscihistory_manager_la_include_HEADERS = \
    includes/gw_history_manager.h \
    includes/HistoryManager.h \
    includes/InitializeHistoryManager.h \
    includes/TerminateHistoryManager.h \
    includes/dynlib_history_manager.h

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/HistoryManagement.i

if SWIG
BUILT_SOURCES = swig
endif

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
