#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=ui_data


#### ui_data : Conf files ####
libsciui_data_la_rootdir = $(mydatadir)
libsciui_data_la_root_DATA =  license.txt version.xml


#### ui_data : init scripts ####
libsciui_data_la_etcdir = $(mydatadir)/etc
libsciui_data_la_etc_DATA = etc/ui_data.quit \
				etc/ui_data.start \
				etc/variablebrowser_menubar.xml \
				etc/variableeditor_menubar.xml

include $(top_srcdir)/Makefile.incl.am


UI_DATA_JNI_SOURCES = src/jni/BrowseVar.cpp \
					  src/jni/EditVar.cpp

UI_DATA_C_SOURCES = 

UI_DATA_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_ui_data.c

GATEWAY_CPP_SOURCES = 	sci_gateway/cpp/sci_editvar.cpp \
						sci_gateway/cpp/sci_browsevar.cpp

libsciui_data_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/core/includes


libsciui_data_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

# Without the ui_data module 
libsciui_data_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

UI_DATA_DISABLE_C_SOURCES = src/noui_data/noui_data.c
libsciui_data_disable_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libsciui_data_disable_la_SOURCES = $(UI_DATA_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libsciui_data-disable.la

if GUI
pkglib_LTLIBRARIES += libsciui_data.la 
endif

libsciui_data_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libsciui_data_la_SOURCES = $(UI_DATA_C_SOURCES) $(UI_DATA_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(UI_DATA_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(UI_DATA_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciui_data_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
BUILT_SOURCES=swig
endif

libsciui_data_la_LIBADD =  \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/jvm/libscijvm.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### ui_data : gateway declaration ####
libsciui_data_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciui_data_la_sci_gateway_DATA = sci_gateway/ui_data_gateway.xml

#### ui_data : icons files ####
libsciui_data_la_iconsdir=$(mydatadir)/
nobase_libsciui_data_la_icons_DATA = images/icons/tp00.gif \
images/icons/tp01.gif \
images/icons/tp02.gif \
images/icons/tp04.gif \
images/icons/tp05.gif \
images/icons/tp06.gif \
images/icons/tp08.gif \
images/icons/tp10.gif \
images/icons/tp15.gif \
images/icons/tp16.gif \
images/icons/tp17.gif

if GUI
USEANT=1
endif
