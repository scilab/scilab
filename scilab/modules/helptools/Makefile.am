# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

if GUI
if BUILD_HELP

USEANT=1

HELPTOOLS_SOURCES = \
    src/jni/BuildDocObject.cpp \
	src/jni/SciDocMain.cpp

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_buildDoc.cpp

GIWS_WRAPPERS = \
    src/jni/BuildDocObject.giws.xml \
	src/jni/BuildDocObjectv2.giws.xml

libscihelptools_la_LIBADD = \
    libscihelptools-algo.la \
    $(top_builddir)/modules/commons/libscicommons.la

libscihelptools_algo_la_SOURCES =  $(HELPTOOLS_SOURCES)
libscihelptools_algo_la_CPPFLAGS = $(libscihelptools_la_CPPFLAGS)
noinst_LTLIBRARIES = libscihelptools-algo.la

HELPTOOLS_DISABLE_CPP_SOURCES = sci_gateway/nogui/nogui.cpp

if GIWS
BUILT_SOURCES=giws
endif

else
GATEWAY_CXX_SOURCES = sci_gateway/nogui/nogui.cpp
endif

else
GATEWAY_CXX_SOURCES = sci_gateway/nogui/nogui.cpp
endif

pkglib_LTLIBRARIES = libscihelptools.la libscihelptools-disable.la

libscihelptools_la_CPPFLAGS= \
	$(JAVA_JNI_INCLUDE) \
	-I$(srcdir)/src/jni/ \
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
	-I$(top_srcdir)/modules/jvm/includes/ \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes/ \
	-I$(top_srcdir)/modules/commons/src/jni \
	-I$(top_srcdir)/modules/dynamic_link/includes \
	-I$(top_srcdir)/modules/string/includes \
	$(AM_CPPFLAGS)

libscihelptools_la_SOURCES =  $(GATEWAY_CXX_SOURCES)

libscihelptools_disable_la_CPPFLAGS = $(libscihelptools_la_CPPFLAGS)

libscihelptools_disable_la_SOURCES =  $(HELPTOOLS_DISABLE_CPP_SOURCES)

# For the code check (splint)
INCLUDE_FLAGS = $(libscihelptools_la_CFLAGS)


#### Target ######
modulename=helptools

#### helptools : Conf files ####
libscihelptools_la_rootdir = $(mydatadir)
libscihelptools_la_root_DATA =  license.txt

#### helptools : init scripts ####
libscihelptools_la_etcdir = $(mydatadir)/etc
libscihelptools_la_etc_DATA = etc/helptools.quit etc/helptools.start etc/MAIN_CHAPTERS etc/fopconf.xml etc/SciDocConf.xml

#### helptools : gateway declaration ####
libscihelptools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihelptools_la_sci_gateway_DATA = sci_gateway/helptools_gateway.xml

#### New documentation generation system
libscihelptools_la_data_configurationdir = $(mydatadir)/data/configuration
libscihelptools_la_data_configuration_DATA = data/configuration/scilab_primitives.txt \
data/configuration/scilab_macros.txt

libscihelptools_la_data_templatedir = $(mydatadir)/data/template
libscihelptools_la_data_template_DATA = data/template/template_chm.html \
data/template/template_html.html \
data/template/template_javahelp.html \
data/template/template_web.html
libscihelptools_la_data_cssdir = $(mydatadir)/data/css
libscihelptools_la_data_css_DATA = data/css/scilab_code.css \
data/css/c_code.css \
data/css/xml_code.css \
data/css/style.css

libscihelptools_la_xsl_fodir = $(mydatadir)/xsl/fo
libscihelptools_la_xsl_fo_DATA = xsl/fo/docbook.xsl


#### helptools : schema files ####
libscihelptools_la_schemadir = $(mydatadir)/schema
libscihelptools_la_schema_DATA = \
schema/docbook.rng \
schema/dbits.rng \
schema/mathml3-common.rng \
schema/docbookxi.rng \
schema/mathml3-content.rng \
schema/mathml3.rng \
schema/scilab.rng \
schema/mathml3-presentation.rng \
schema/extendedStyle.xsl

#### helptools : pages files ####
libscihelptools_la_data_pagesdir = $(mydatadir)/data/pages
libscihelptools_la_data_pages_DATA = \
data/pages/error.html \
data/pages/homepage-en_US.html \
data/pages/homepage-fr_FR.html \
data/pages/ban-en_US.png \
data/pages/ban-fr_FR.png

mostlyclean-local:
	rm -rf web javaHelp html pdf

include $(top_srcdir)/Makefile.incl.am

