##
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
##  Copyright (C) 2007-2008 - DIGITEO - Antoine ELIAS
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
##
##

#### Target ######
modulename=coverage

####io : init scripts ####
libscicoverage_la_etcdir = $(mydatadir)/etc
libscicoverage_la_etc_DATA = \
    etc/coverage.quit \
    etc/coverage.start

include $(top_srcdir)/Makefile.incl.am

COVERAGE_CXX_SOURCES = \
    src/cpp/CoverModule.cpp \
    src/cpp/InstrumentVisitor.cpp \
    src/cpp/CoverResult.cpp \
    src/cpp/CodePrinterVisitor.cpp \
    src/cpp/CovHTMLCodePrinter.cpp \
    src/cpp/CoverMacroInfo.cpp \
    src/cpp/URLEncoder.cpp \
    src/cpp/CoverModule_interface.cpp

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_covStart.cpp \
    sci_gateway/cpp/sci_covWrite.cpp \
    sci_gateway/cpp/sci_covStop.cpp \
    sci_gateway/cpp/sci_covMerge.cpp \
    sci_gateway/cpp/coverage_gw.cpp

libscicoverage_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/fileio/src/c/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/threads/includes/ \
    $(XML_FLAGS) \
    $(COVERAGE_FLAGS) \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscicoverage.la
noinst_LTLIBRARIES = libscicoverage-algo.la

libscicoverage_algo_la_SOURCES = $(COVERAGE_CXX_SOURCES)
libscicoverage_la_SOURCES = $(GATEWAY_CXX_SOURCES)
libscicoverage_algo_la_CPPFLAGS = $(libscicoverage_la_CPPFLAGS)

libscicoverage_la_LDFLAGS = $(AM_LDFLAGS)
libscicoverage_la_LIBADD = libscicoverage-algo.la $(XML_LIBS)

# For the code check (splint)
INCLUDE_FLAGS = $(libscicoverage_la_CPPFLAGS)

#### coverage : include files ####
libscicoverage_la_includedir=$(pkgincludedir)

#### coverage : gateway declaration ####
libscicoverage_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscicoverage_la_sci_gateway_DATA =
