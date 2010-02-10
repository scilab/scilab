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
modulename=graphic_objects


#### graphic_objects : Conf files ####
libscigraphic_objects_la_rootdir = $(mydatadir)
libscigraphic_objects_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### graphic_objects : init scripts ####
libscigraphic_objects_la_etcdir = $(mydatadir)/etc
libscigraphic_objects_la_etc_DATA = etc/graphic_objects.quit \
				etc/graphic_objects.start

include $(top_srcdir)/Makefile.incl.am


GRAPHIC_OBJECTS_JNI_SOURCES = 

GRAPHIC_OBJECTS_C_SOURCES = 

GRAPHIC_OBJECTS_CPP_SOURCES = 

GIWS_WRAPPERS = 

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_graphic_objects.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_editvar.cpp

libscigraphic_objects_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes \
			-I$(top_srcdir)/modules/core/includes


libscigraphic_objects_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/jni/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

# Without the graphic_objects module 
libscigraphic_objects_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

GRAPHIC_OBJECTS_DISABLE_C_SOURCES = src/nographic_objects/nographic_objects.c
libscigraphic_objects_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscigraphic_objects_disable_la_SOURCES = $(GRAPHIC_OBJECTS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscigraphic_objects-disable.la

if GUI
pkglib_LTLIBRARIES += libscigraphic_objects.la 
endif

libscigraphic_objects_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscigraphic_objects_la_SOURCES = $(GRAPHIC_OBJECTS_C_SOURCES) $(GRAPHIC_OBJECTS_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(GRAPHIC_OBJECTS_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GRAPHIC_OBJECTS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigraphic_objects_la_CFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = 

if SWIG
BUILT_SOURCES=swig
endif

libscigraphic_objects_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### graphic_objects : gateway declaration ####
libscigraphic_objects_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraphic_objects_la_sci_gateway_DATA = sci_gateway/graphic_objects_gateway.xml

if GUI
USEANT=1
endif
