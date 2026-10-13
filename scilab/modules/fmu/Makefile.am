#
# Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2026 - Dassault Systèmes - Clément DAVID
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=fmu

#### fmu : Conf files ####
libscifmu_la_rootdir = $(mydatadir)
libscifmu_la_root_DATA =  license.txt

#### fmu : init scripts & configuration files ####
libscifmu_la_etcdir = $(mydatadir)/etc
libscifmu_la_etc_DATA = \
    etc/fmu.quit \
    etc/fmu.start

include $(top_srcdir)/Makefile.incl.am

FMU_C_SOURCES = \
    src/c/fmiPlatformTypes.h \
    src/c/fmiFunctionTypes.h \
    src/c/fmiFunctions.h \
    src/c/fmi2TypesPlatform.h \
    src/c/fmi2FunctionTypes.h \
    src/c/fmi2Functions.h \
    src/c/fmi3PlatformTypes.h \
    src/c/fmi3FunctionTypes.h \
    src/c/fmi3Functions.h

FMU_CXX_SOURCES = \
    src/cpp/fmu_wrapper.cpp

FMU_SWIG_CXX_SOURCES = \
    src/swig/fmuswig_wrap.cxx

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/fmu_link.cpp \
    sci_gateway/cpp/fmu_ulink.cpp

# Regenerate src/swig/fmuswig_wrap.cxx, fmuswig_gateway.xml and loader.sce
SWIG_SCILAB_WRAPPERS = src/swig/fmuswig.i
SWIG_OPTIONS = -c++ -gatewayxml6 -I$(srcdir)/src/c -I$(srcdir)/includes

libscifmu_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/swig/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/mexlib/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

pkglib_LTLIBRARIES = libscifmu.la
noinst_LTLIBRARIES = libscifmu-algo.la

libscifmu_algo_la_SOURCES = $(FMU_CXX_SOURCES) $(FMU_C_SOURCES)
libscifmu_algo_la_CPPFLAGS = $(libscifmu_la_CPPFLAGS)
libscifmu_la_SOURCES = $(GATEWAY_CXX_SOURCES) $(FMU_SWIG_CXX_SOURCES)

libscifmu_la_LDFLAGS = $(AM_LDFLAGS)
libscifmu_la_LIBADD = libscifmu-algo.la

# For the code check (splint)
INCLUDE_FLAGS = $(libscifmu_la_CPPFLAGS)

#### fmu : include files ####
libscifmu_la_includedir=$(pkgincludedir)
libscifmu_la_include_HEADERS = \
    includes/dynlib_fmucpp_wrapper.h \
    includes/dynlib_fmu_scilab.h \
    includes/fmiModelFunctions.h \
    includes/fmiModelTypes.h \
    includes/gw_fmu.h \
    includes/fmu_wrapper.hxx

#### fmu : gateway declaration ####
libscifmu_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscifmu_la_sci_gateway_DATA = sci_gateway/fmu_gateway.xml

#### fmu : swig files ####
libscifmu_la_src_swigdir = $(mydatadir)/src/swig
libscifmu_la_src_swig_DATA = src/swig/fmuswig_gateway.xml

#### fmu : FMI 1.0 headers copied next to the sources by exportFMU ####
libscifmu_la_fmi_includesdir = $(mydatadir)/includes
libscifmu_la_fmi_includes_DATA = \
    includes/fmiModelFunctions.h \
    includes/fmiModelTypes.h
