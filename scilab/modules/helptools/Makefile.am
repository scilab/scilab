# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre LEDRU
#
# This file is distributed under the same license as the Scilab package.

if GUI
if BUILD_HELP

USEANT=1

HELPTOOLS_SOURCES = src/jni/BuildDocObject.cpp \
	src/jni/SciDocMain.cpp \
	src/jni/GiwsException.cpp

GATEWAY_C_SOURCES = sci_gateway/c/gw_helptools.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_buildDoc.cpp \
	sci_gateway/cpp/sci_buildDocv2.cpp

GIWS_WRAPPERS = src/jni/BuildDocObject.giws.xml \
	src/jni/BuildDocObjectv2.giws.xml

libscihelptools_la_LIBADD = $(top_builddir)/modules/jvm/libscijvm.la

if GIWS
BUILT_SOURCES=giws
endif

else
GATEWAY_C_SOURCES = sci_gateway/nogui/nogui.c
endif

else
GATEWAY_C_SOURCES = sci_gateway/nogui/nogui.c
endif

pkglib_LTLIBRARIES = libscihelptools.la

libscihelptools_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/output_stream/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/jvm/includes


libscihelptools_la_CPPFLAGS= $(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/src/jni/ \
				-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes/

libscihelptools_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

libscihelptools_la_SOURCES =  $(GATEWAY_C_SOURCES) $(HELPTOOLS_SOURCES) $(GATEWAY_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscihelptools_la_CFLAGS)


#### Target ######
modulename=helptools

#### helptools : Conf files ####
libscihelptools_la_rootdir = $(mydatadir)
libscihelptools_la_root_DATA =  license.txt version.xml

#### helptools : init scripts ####
libscihelptools_la_etcdir = $(mydatadir)/etc
libscihelptools_la_etc_DATA = etc/helptools.quit etc/helptools.start etc/MAIN_CHAPTERS etc/fopconf.xml etc/SciDocConf.xml

#### helptools : bin files ####
libscihelptools_la_bindir = $(mydatadir)/bin
libscihelptools_la_bin_DATA = bin/scivalid \
bin/man2sci \
bin/manrev2sci

#### helptools : gateway declaration ####
libscihelptools_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscihelptools_la_sci_gateway_DATA = sci_gateway/helptools_gateway.xml

#### New documentation generation system
libscihelptools_la_data_configurationdir = $(mydatadir)/data/configuration
libscihelptools_la_data_configuration_DATA = data/configuration/scilab_primitives.txt \
data/configuration/scilab_macros.txt

libscihelptools_la_data_templatedir = $(mydatadir)/data/template
libscihelptools_la_data_template_DATA = data/template/template_javahelp.html
libscihelptools_la_data_cssdir = $(mydatadir)/data/css
libscihelptools_la_data_css_DATA = data/css/scilab_code.css \
data/css/c_code.css \
data/css/xml_code.css \
data/css/style.css

#### helptools : xsl files ####
libscihelptools_la_xsl_convertdir = $(mydatadir)/xsl/convert
libscihelptools_la_xsl_convert_DATA = \
xsl/convert/man2manrev.xsl \
xsl/convert/manrev2sci.xsl

libscihelptools_la_xsl_fodir = $(mydatadir)/xsl/fo
libscihelptools_la_xsl_fo_DATA = xsl/fo/docbook.xsl


#### helptools : schema files ####
libscihelptools_la_schemadir = $(mydatadir)/schema
libscihelptools_la_schema_DATA = schema/scilab.rnc \
schema/extendedStyle.xsl

#### helptools : pages files ####
libscihelptools_la_data_pagesdir = $(mydatadir)/data/pages
libscihelptools_la_data_pages_DATA = data/pages/error.html \
data/pages/homepage-en_US.html \
data/pages/homepage-fr_FR.html \
data/pages/ban-en_US.png \
data/pages/ban-fr_FR.png

include $(top_srcdir)/Makefile.incl.am

