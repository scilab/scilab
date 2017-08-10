#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009-2010 - DIGITEO
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=scinotes


#### scinotes : Conf files ####
libsciscinotes_la_rootdir = $(mydatadir)
libsciscinotes_la_root_DATA =  license.txt


#### scinotes : init scripts & configuration files ####
libsciscinotes_la_etcdir = $(mydatadir)/etc
libsciscinotes_la_etc_DATA = \
	etc/scinotesConfiguration.xml \
	etc/keysConfiguration.xml \
	etc/scinotesGUIConfiguration.xml \
	etc/scinotes.quit \
	etc/scinotes.start \
	etc/XConfiguration-scinotes.xml \
	etc/XConfiguration-scinotes.xsl

SCINOTES_JNI_SOURCES = src/jni/ScilabKeywords_wrap.c

SCINOTES_C_SOURCES = src/c/ScilabKeywords.c

SCINOTES_CXX_SOURCES = \
    src/jni/SciNotes.cpp \
    src/cpp/callscinotes.cpp

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/sci_scinotes.cpp \
    sci_gateway/cpp/sci_closeSciNotesFromScilab.cpp

libsciscinotes_la_CPPFLAGS = \
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
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/functions/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    $(JAVA_JNI_INCLUDE) \
    $(AM_CPPFLAGS)

# Without the scinotes module
libsciscinotes_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    $(AM_CPPFLAGS)

SCINOTES_DISABLE_C_SOURCES = src/noscinotes/noscinotes.c
SCINOTES_DISABLE_CPP_SOURCES = src/noscinotes/noscinotes_gw.cpp
libsciscinotes_disable_la_SOURCES = $(SCINOTES_DISABLE_C_SOURCES) $(SCINOTES_DISABLE_CPP_SOURCES)

pkglib_LTLIBRARIES = libsciscinotes-disable.la

if GUI
    pkglib_LTLIBRARIES += libsciscinotes.la
    noinst_LTLIBRARIES = libsciscinotes-algo.la
endif

libsciscinotes_la_LDFLAGS = $(AM_LDFLAGS)

libsciscinotes_algo_la_SOURCES = $(SCINOTES_C_SOURCES) $(SCINOTES_JNI_SOURCES) $(SCINOTES_CXX_SOURCES)
libsciscinotes_la_SOURCES = $(GATEWAY_CXX_SOURCES)
libsciscinotes_algo_la_CPPFLAGS = $(libsciscinotes_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(SCINOTES_C_SOURCES)
INCLUDE_FLAGS = $(libsciscinotes_la_CPPFLAGS)

GIWS_WRAPPERS = src/jni/SciNotes.giws.xml
SWIG_WRAPPERS = src/jni/ScilabKeywords.i

BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif
if SWIG
BUILT_SOURCES+=swig
endif

libsciscinotes_la_LIBADD = libsciscinotes-algo.la $(X_LIBS) $(X_EXTRA_LIBS)

#### scinotes : gateway declaration ####
libsciscinotes_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciscinotes_la_sci_gateway_DATA = sci_gateway/scinotes_gateway.xml

if GUI
if BUILD_HELP
    USEANT=1
endif
endif

include $(top_srcdir)/Makefile.incl.am
