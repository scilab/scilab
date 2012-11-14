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

BUILT_SOURCES = 	includes/graphicObjectsProperties.h
#This file is also generated, but can not be listed to prevent GCJ call.
#src/java/org/scilab/modules/graphic_objects/graphicObject/GraphicObjectProperties.java

GRAPHIC_OBJECTS_JNI_SOURCES = src/jni/CallGraphicController.cpp \
                              src/jni/GraphicObjectBuilder.cpp

GRAPHIC_OBJECTS_C_SOURCES = src/jni/DataLoader_wrap.c \
							src/jni/ScilabNativeView_wrap.c

GRAPHIC_OBJECTS_CXX_SOURCES = 	src/cpp/createGraphicObject.cpp \
								src/cpp/getGraphicObjectProperty.cpp \
								src/cpp/getConsoleIdentifier.cpp \
								src/cpp/setGraphicObjectProperty.cpp \
								src/cpp/deleteGraphicObject.cpp \
								src/cpp/dataModel.cpp \
								src/cpp/Data3D.cpp \
								src/cpp/DataLoader.cpp \
								src/cpp/NgonData.cpp \
								src/cpp/NgonGeneralData.cpp \
								src/cpp/NgonGridData.cpp \
								src/cpp/NgonGridMatplotData.cpp \
								src/cpp/NgonPolylineData.cpp \
								src/cpp/TriangleMeshData.cpp \
								src/cpp/TriangleMeshFecData.cpp \
								src/cpp/ColorComputer.cpp \
								src/cpp/DecompositionUtils.cpp \
								src/cpp/Fac3DColorComputer.cpp \
								src/cpp/Fac3DDecomposer.cpp \
								src/cpp/NgonGridDataDecomposer.cpp \
								src/cpp/NgonGridGrayplotDataDecomposer.cpp \
								src/cpp/MatPlotDecomposer.cpp \
								src/cpp/NgonGridMatplotDataDecomposer.cpp \
								src/cpp/Plot3DDecomposer.cpp \
								src/cpp/PolylineDecomposer.cpp \
								src/cpp/TriangleMeshFecDataDecomposer.cpp \
								src/cpp/Triangulator.cpp \
								src/cpp/ScilabView.cpp \
								src/cpp/FigureList.cpp \
								src/cpp/HandleManagement.cpp \
								src/cpp/AxesModel.cpp \
								src/cpp/FigureModel.cpp \
								src/cpp/CurrentFigure.cpp \
								src/cpp/CurrentObject.cpp \
								src/cpp/CurrentSubwin.cpp

GIWS_WRAPPERS = src/jni/graphic_objects.giws.xml

if GIWS
BUILT_SOURCES += giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_graphic_objects.c

GATEWAY_CXX_SOURCES = sci_gateway/cpp/sci_graphicfunction.cpp

libscigraphic_objects_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/jni/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/commons/src/jni/ \
			-I$(top_srcdir)/modules/core/includes/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			$(AM_CPPFLAGS)

# Without the graphic_objects module
libscigraphic_objects_disable_la_CPPFLAGS = -I$(srcdir)/includes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			$(AM_CPPFLAGS)

GRAPHIC_OBJECTS_DISABLE_C_SOURCES = src/nographic_objects/nographic_objects.c
libscigraphic_objects_disable_la_SOURCES = $(GRAPHIC_OBJECTS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscigraphic_objects-disable.la

if GUI
pkglib_LTLIBRARIES += libscigraphic_objects.la
endif

libscigraphic_objects_la_LDFLAGS = $(X_LIBS) $(X_EXTRA_LIBS) $(AM_LDFLAGS)

libscigraphic_objects_la_SOURCES = $(GRAPHIC_OBJECTS_GEN_SOURCES) $(GRAPHIC_OBJECTS_C_SOURCES) $(GRAPHIC_OBJECTS_JNI_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES) $(GRAPHIC_OBJECTS_CXX_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GRAPHIC_OBJECTS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigraphic_objects_la_CPPFLAGS)


#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/DataLoader.i \
				src/jni/ScilabNativeView.i
if SWIG
  BUILT_SOURCES += swig
endif

libscigraphic_objects_la_LIBADD = $(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### graphic_objects : gateway declaration ####
libscigraphic_objects_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraphic_objects_la_sci_gateway_DATA = sci_gateway/graphic_objects_gateway.xml

includes/graphicObjectsProperties.h: Makefile.am src/scripts/propertiesMap.properties
	@$(top_srcdir)/modules/graphic_objects/src/scripts/genPropertiesName.sh --C $(top_srcdir)/modules/graphic_objects/src/scripts/propertiesMap.properties $(top_srcdir)/modules/graphic_objects/includes/graphicObjectProperties.h
	@$(top_srcdir)/modules/graphic_objects/src/scripts/genPropertiesName.sh --java $(top_srcdir)/modules/graphic_objects/src/scripts/propertiesMap.properties $(top_srcdir)/modules/graphic_objects/src/java/org/scilab/modules/graphic_objects/graphicObject/GraphicObjectProperties.java

if GUI
USEANT=1
endif
