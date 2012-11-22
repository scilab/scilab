#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=uiwidget


#### uiwidget : Conf files ####
libsciuiwidget_la_rootdir = $(mydatadir)
libsciuiwidget_la_root_DATA =  license.txt 


#### scinotes : init scripts & configuration files ####
libsciuiwidget_la_etcdir = $(mydatadir)/etc
libsciuiwidget_la_etc_DATA = etc/uiwidget.quit \
	                     etc/uiwidget.start

UIWIDGET_JNI_SOURCES = src/jni/UIWidget.cpp \
		       src/jni/UserData_wrap.c

UIWIDGET_C_SOURCES = 

UIWIDGET_CXX_SOURCES = src/cpp/UIWidgetTools.cpp \
	               src/cpp/UserDataHandler.cpp

GATEWAY_C_SOURCES = sci_gateway/c/gw_uiwidget.c

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_uiwidget.cpp \
		      sci_gateway/cpp/sci_uiget.cpp \
		      sci_gateway/cpp/sci_uiset.cpp \
		      sci_gateway/cpp/sci_percent_UIWidget_e.cpp \
	              sci_gateway/cpp/sci_percent_foo_i_UIWidget.cpp

libsciuiwidget_la_CPPFLAGS = -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/libs/MALLOC/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/types/includes/ \
    $(JAVA_JNI_INCLUDE) \
    $(UIWIDGET_FLAGS) \
    $(AM_CPPFLAGS)


pkglib_LTLIBRARIES = libsciuiwidget.la 
noinst_LTLIBRARIES = libsciuiwidget-algo.la 

libsciuiwidget_la_LDFLAGS = $(X_LIBS) $(X_EXTRA_LIBS) $(AM_LDFLAGS)

libsciuiwidget_algo_la_SOURCES = $(UIWIDGET_C_SOURCES) $(UIWIDGET_JNI_SOURCES) $(UIWIDGET_CXX_SOURCES)
libsciuiwidget_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES) 
libsciuiwidget_algo_la_CPPFLAGS = $(libsciuiwidget_la_CPPFLAGS)

libsciuiwidget_la_LIBADD = libsciuiwidget-algo.la

# For the code check (splint)
CHECK_SRC= $(UIWIDGET_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciuiwidget_la_CPPFLAGS)

#### uiwidget : include files ####
libsciuiwidget_la_includedir=$(pkgincludedir)

#### uiwidget : gateway declaration ####
libsciuiwidget_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciuiwidget_la_sci_gateway_DATA = sci_gateway/uiwidget_gateway.xml

if GUI
if BUILD_HELP
    USEANT=1
endif
endif

include $(top_srcdir)/Makefile.incl.am