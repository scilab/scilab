#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2009 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=xpad


#### xpad : Conf files ####
libscixpad_la_rootdir = $(mydatadir)
libscixpad_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### xpad : init scripts ####
libscixpad_la_etcdir = $(mydatadir)/etc
libscixpad_la_etc_DATA = 	etc/xpadConfiguration.xml \
				etc/xpad.quit \
				etc/xpad.start

include $(top_srcdir)/Makefile.incl.am


XPAD_JNI_SOURCES = src/jni/ScilabKeywords_wrap.c

XPAD_C_SOURCES = src/c/ScilabKeywords.c

XPAD_CPP_SOURCES = src/jni/Xpad.cpp src/cpp/callxpad.cpp

GIWS_WRAPPERS = src/jni/Xpad.giws.xml

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_xpad.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_xpad.cpp \
sci_gateway/cpp/sci_xpad_highlightline.cpp

libscixpad_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/core/includes \
			-I$(top_srcdir)/modules/functions/includes

libscixpad_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/fileio/includes \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

# Without the xpad module 
libscixpad_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

XPAD_DISABLE_C_SOURCES = src/noxpad/noxpad.c
libscixpad_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscixpad_disable_la_SOURCES = $(XPAD_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscixpad-disable.la

if GUI
pkglib_LTLIBRARIES += libscixpad.la 
endif

libscixpad_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscixpad_la_SOURCES = $(XPAD_C_SOURCES) $(XPAD_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(XPAD_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(XPAD_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscixpad_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/ScilabKeywords.i 

if SWIG
BUILT_SOURCES=swig
endif

libscixpad_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### xpad : gateway declaration ####
libscixpad_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixpad_la_sci_gateway_DATA = sci_gateway/xpad_gateway.xml

if GUI
USEANT=1
endif
