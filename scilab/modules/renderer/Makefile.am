#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2007 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009 - Pierre MARECHAL
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#


#### Target ####
modulename=renderer

#### renderer : Conf files ####
libscirenderer_la_rootdir = $(mydatadir)
libscirenderer_la_root_DATA =  license.txt

#### renderer : init scripts ####
libscirenderer_la_etcdir = $(mydatadir)/etc
libscirenderer_la_etc_DATA = etc/renderer.quit etc/renderer.start



# List of the c files
RENDERER_CPP_SOURCES = \
src/cpp/getHandleDrawer.cpp \
src/cpp/HandleObserverManagement.cpp \
src/cpp/DrawableObjectFactory.cpp \
src/cpp/HandleDrawingObserver.cpp \
src/cpp/BuildDrawingObserver.cpp \
src/cpp/DrawableObjectJoGL.cpp \
src/cpp/GetJavaProperty.cpp \
src/cpp/SetJavaProperty.cpp \
src/cpp/JavaInteraction.cpp \
src/cpp/DrawableObject.cpp \
src/cpp/DrawingBridge.cpp \
src/cpp/GraphicSynchronization/GlobalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerBridgeFactory.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerFactory.cpp \
src/cpp/GraphicSynchronization/LocalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerInterface.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerJavaMapper.cpp \
src/cpp/BasicAlgos.cpp \
src/jni/GraphicSynchronizerJava.cpp \
src/cpp/RendererFontManager.cpp \
src/jni/XlFontManager.cpp \
src/cpp/rendererBasicAlgos.cpp \
src/jni/CallRenderer.cpp

# Includes need for the compilation
libscirenderer_la_CPPFLAGS =  	$(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/commons/src/jni \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/graphics/includes/handleDrawing/ \
				-I$(top_srcdir)/libs/doublylinkedlist/includes \
				-I$(top_srcdir)/modules/output_stream/includes \
				-I$(top_srcdir)/modules/graphic_export/includes/ \
				-I$(top_srcdir)/modules/elementary_functions/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/gui/includes/

libscirenderer_la_CFLAGS=	-I$(srcdir)/includes/

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = $(RENDERER_CPP_SOURCES)

SWIG_WRAPPERS = src/jni/FigureScilabCall.i

if SWIG
  BUILT_SOURCES=swig
endif

# For the code check (splint)
INCLUDE_FLAGS = $(libscirenderer_la_CFLAGS)

# needs the rendererer before $(top_builddir)/modules/jvm/libscijvm.la


GIWS_WRAPPERS= \
src/jni/BackTrihedronDrawerGL.giws.xml \
src/jni/GraphicSynchronizerJava.giws.xml \
src/jni/RenderingChecker.giws.xml \
src/jni/XlFontManager.giws.xml \
src/jni/SubwinBackgroundDrawerGL.giws.xml \
src/jni/renderer.giws.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1


if GIWS
BUILT_SOURCES=giws
endif

if GUI
USEANT=1
pkglib_LTLIBRARIES = libscirenderer.la
endif

libscirenderer_la_LDFLAGS = -version-number $(SCILAB_LIBRARY_VERSION)

include $(top_srcdir)/Makefile.incl.am
