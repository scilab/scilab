#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=preferences


#### preferences : Conf files ####
libscipreferences_la_rootdir = $(mydatadir)
libscipreferences_la_root_DATA =  license.txt 


#### preferences : init scripts & configuration files ####
libscipreferences_la_etcdir = $(mydatadir)/etc
libscipreferences_la_etc_DATA = \
	etc/preferences.quit \
	etc/preferences.start \
	etc/XConfiguration.xml \
	etc/XWizard-expression.xml \
	etc/XWizard-function.xml \
	etc/XWizard-xcos.xml \
	etc/XWizard.xml

#### preferences : xslt files ####
libscipreferences_la_src_xsltdir = $(mydatadir)/src/xslt
libscipreferences_la_src_xslt_DATA = \
	src/xslt/XCommon-toolbars.xsl \
	src/xslt/XCommon.xsl \
	src/xslt/XConfiguration.xsl \
	src/xslt/XGeneric.xsl \
	src/xslt/XWizard.xsl
libscipreferences_la_src_xslt_xwizarddir = $(mydatadir)/src/xslt/XWizard
libscipreferences_la_src_xslt_xwizard_DATA = \
	src/xslt/XWizard/XWizard-any.xsl \
	src/xslt/XWizard/XWizard-expression.xsl \
	src/xslt/XWizard/XWizard-function.xsl \
	src/xslt/XWizard/XWizard-xcos.xsl

include $(top_srcdir)/Makefile.incl.am

PREFERENCES_JNI_SOURCES = src/jni/ScilabPreferences.cpp

PREFERENCES_C_SOURCES =

PREFERENCES_CPP_SOURCES =

GIWS_WRAPPERS = src/jni/ScilabPreferences.giws.xml

if GIWS
    BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_preferences.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_addModulePreferences.cpp \
                      sci_gateway/cpp/sci_removeModulePreferences.cpp

libscipreferences_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
    -I$(top_srcdir)/libs/MALLOC/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/core/includes \
    -I$(top_srcdir)/modules/functions/includes

libscipreferences_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/fileio/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/api_scilab/includes

# Without the preferences module 
libscipreferences_disable_la_CFLAGS = -Iincludes/ \
    -I$(top_srcdir)/modules/output_stream/includes

PREFERENCES_DISABLE_C_SOURCES = src/nopreferences/nopreferences.c
libscipreferences_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscipreferences_disable_la_SOURCES = $(PREFERENCES_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscipreferences-disable.la

if GUI
    pkglib_LTLIBRARIES += libscipreferences.la 
    noinst_LTLIBRARIES = libscipreferences-algo.la 
endif

libscipreferences_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscipreferences_algo_la_SOURCES = $(PREFERENCES_C_SOURCES) $(PREFERENCES_JNI_SOURCES) $(PREFERENCES_CPP_SOURCES)
libscipreferences_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) 
libscipreferences_algo_la_CFLAGS = $(libscipreferences_la_CFLAGS)
libscipreferences_algo_la_CPPFLAGS = $(libscipreferences_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(PREFERENCES_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscipreferences_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
    BUILT_SOURCES=swig
endif

libscipreferences_la_LIBADD = libscipreferences-algo.la \
    $(top_builddir)/libs/MALLOC/libscimalloc.la \
    $(top_builddir)/modules/jvm/libscijvm.la \
    $(top_builddir)/modules/output_stream/libscioutput_stream.la


#### preferences : gateway declaration ####
libscipreferences_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscipreferences_la_sci_gateway_DATA = sci_gateway/preferences_gateway.xml

if GUI
    USEANT=1
endif
