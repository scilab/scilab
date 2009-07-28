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
modulename=xcos


#### xcos : Conf files ####
libscixcos_la_rootdir = $(mydatadir)
libscixcos_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### xcos : init scripts ####
libscixcos_la_etcdir = $(mydatadir)/etc
libscixcos_la_etc_DATA = etc/xcos.quit \
etc/xcos.start

include $(top_srcdir)/Makefile.incl.am

XCOS_C_SOURCES = 

XCOS_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_xcos.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_Xcos.cpp

libscixcos_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

libscixcos_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

# Without the xcos module 
libscixcos_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

XCOS_DISABLE_C_SOURCES = src/noxcos/noxcos.c
libscixcos_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscixcos_disable_la_SOURCES = $(XCOS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscixcos-disable.la

if GUI
pkglib_LTLIBRARIES += libscixcos.la 
endif

libscixcos_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscixcos_la_SOURCES = $(XCOS_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(XCOS_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(XCOS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscixcos_la_CFLAGS)



libscixcos_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### xcos : gateway declaration ####
libscixcos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixcos_la_sci_gateway_DATA = sci_gateway/xcos_gateway.xml


#### xcos : include files ####
libscixcos_la_includedir=$(pkgincludedir)/xcos
libscixcos_la_include_HEADERS = includes/gw_xcos.h \
includes/dynlib_xcos.h

if GUI
USEANT=1
endif
