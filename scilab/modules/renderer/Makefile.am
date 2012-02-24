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
src/cpp/subwinDrawing/DrawableSubwinFactory.cpp \
src/cpp/subwinDrawing/DrawableSubwin.cpp \
src/cpp/compoundDrawing/DrawableCompoundFactory.cpp \
src/cpp/compoundDrawing/DrawableCompound.cpp \
src/cpp/segsDrawing/DrawableSegs.cpp \
src/cpp/segsDrawing/DrawableSegsFactory.cpp \
src/cpp/surfaceDrawing/DrawableSurfaceFactory.cpp \
src/cpp/surfaceDrawing/DrawableSurface.cpp \
src/cpp/axesDrawing/DrawableAxes.cpp \
src/cpp/axesDrawing/DrawableAxesFactory.cpp \
src/cpp/grayplotDrawing/DrawableGrayplot.cpp \
src/cpp/grayplotDrawing/DrawableGrayplotFactory.cpp \
src/cpp/getHandleDrawer.cpp \
src/cpp/HandleObserverManagement.cpp \
src/cpp/DrawableObjectFactory.cpp \
src/cpp/DrawableClippedObject.cpp \
src/cpp/HandleDrawingObserver.cpp \
src/cpp/BuildDrawingObserver.cpp \
src/cpp/DrawableObjectJoGL.cpp \
src/cpp/GetJavaProperty.cpp \
src/cpp/SetJavaProperty.cpp \
src/cpp/JavaInteraction.cpp \
src/cpp/DrawableObject.cpp \
src/cpp/DrawingBridge.cpp \
src/cpp/subwinDrawing/CameraJoGL.cpp \
src/cpp/subwinDrawing/CameraBridge.cpp \
src/cpp/subwinDrawing/DrawableSubwinBridgeFactory.cpp \
src/cpp/subwinDrawing/CameraFactory.cpp \
src/cpp/subwinDrawing/DrawableSubwinJoGL.cpp \
src/cpp/subwinDrawing/Camera.cpp \
src/cpp/subwinDrawing/CameraBridgeFactory.cpp \
src/cpp/DrawableClippedObjectJoGL.cpp \
src/cpp/GraphicSynchronization/GlobalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerBridgeFactory.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerFactory.cpp \
src/cpp/GraphicSynchronization/LocalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerInterface.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerJavaMapper.cpp \
src/cpp/subwinDrawing/DrawableSubwinJavaMapper.cpp \
src/jni/DrawableSubwinGL.cpp \
src/jni/IsoViewCameraGL.cpp \
src/jni/IsometricCameraGL.cpp \
src/cpp/subwinDrawing/IsoViewCameraJavaMapper.cpp \
src/cpp/subwinDrawing/IsometricCameraJavaMapper.cpp \
src/cpp/subwinDrawing/LinearBoundsComputer.cpp \
src/cpp/subwinDrawing/ConcreteDrawableSubwin.cpp \
src/cpp/subwinDrawing/LogarithmicBoundsComputer.cpp \
src/jni/BackTrihedronDrawerGL.cpp \
src/cpp/subwinDrawing/BackTrihedronDrawerJoGL.cpp \
src/cpp/subwinDrawing/BackTrihedronDrawerJavaMapper.cpp \
src/jni/FullBoxDrawerGL.cpp \
src/jni/HalfBoxDrawerGL.cpp \
src/cpp/subwinDrawing/HalfBoxDrawerJavaMapper.cpp \
src/cpp/subwinDrawing/HalfBoxDrawerJoGL.cpp \
src/cpp/subwinDrawing/FullBoxDrawerJavaMapper.cpp \
src/cpp/subwinDrawing/FullBoxDrawerJoGL.cpp \
src/cpp/subwinDrawing/UserDefinedTicksComputer.cpp \
src/cpp/subwinDrawing/TicksDrawer.cpp \
src/cpp/subwinDrawing/TicksDrawerFactory.cpp \
src/cpp/subwinDrawing/TicksDrawerJoGL.cpp \
src/cpp/BasicAlgos.cpp \
src/cpp/subwinDrawing/AutomaticTicksComputer.cpp \
src/jni/GraphicSynchronizerJava.cpp \
src/cpp/subwinDrawing/UserDefLogTicksComputer.cpp \
src/cpp/subwinDrawing/AutoLogTicksComputer.cpp \
src/cpp/subwinDrawing/GridDrawerJoGL.cpp \
src/cpp/subwinDrawing/YGridDrawerJoGL.cpp \
src/cpp/subwinDrawing/GridDrawer.cpp \
src/cpp/subwinDrawing/XGridDrawerJoGL.cpp \
src/cpp/subwinDrawing/ZGridDrawerJoGL.cpp \
src/cpp/surfaceDrawing/ConcreteDrawableSurface.cpp \
src/cpp/surfaceDrawing/DrawableSurfaceJavaMapper.cpp \
src/cpp/surfaceDrawing/DrawableSurfaceJoGL.cpp \
src/cpp/surfaceDrawing/SurfaceFacetDrawerJavaMapper.cpp \
src/cpp/surfaceDrawing/SurfaceFacetDrawerJoGL.cpp \
src/cpp/surfaceDrawing/SurfaceLineDrawerJavaMapper.cpp \
src/cpp/surfaceDrawing/SurfaceLineDrawerJoGL.cpp \
src/cpp/surfaceDrawing/SurfaceMarkDrawerJavaMapper.cpp \
src/cpp/surfaceDrawing/SurfaceMarkDrawerJoGL.cpp \
src/cpp/surfaceDrawing/DrawableSurfaceBridgeFactory.cpp \
src/jni/DrawableSurfaceGL.cpp \
src/jni/SurfaceFacetDrawerGL.cpp \
src/jni/SurfaceLineDrawerGL.cpp \
src/jni/SurfaceMarkDrawerGL.cpp \
src/jni/DrawableSegsGL.cpp \
src/jni/SegsArrowDrawerGL.cpp \
src/jni/SegsLineDrawerGL.cpp \
src/jni/SegsMarkDrawerGL.cpp \
src/cpp/segsDrawing/ChampDecomposer.cpp \
src/cpp/segsDrawing/ConcreteDrawableSegs.cpp \
src/cpp/segsDrawing/DrawableSegsBridgeFactory.cpp \
src/cpp/segsDrawing/DrawableSegsJavaMapper.cpp \
src/cpp/segsDrawing/DrawableSegsJoGL.cpp \
src/cpp/segsDrawing/SegsArrowDrawerJavaMapper.cpp \
src/cpp/segsDrawing/SegsArrowDrawerJoGL.cpp \
src/cpp/segsDrawing/SegsDecomposer.cpp \
src/cpp/segsDrawing/SegsLineDrawerJavaMapper.cpp \
src/cpp/segsDrawing/SegsLineDrawerJoGL.cpp \
src/cpp/segsDrawing/SegsMarkDrawerJavaMapper.cpp \
src/cpp/segsDrawing/SegsMarkDrawerJoGL.cpp \
src/jni/DrawableGrayplotGL.cpp \
src/cpp/grayplotDrawing/ConcreteDrawableGrayplot.cpp \
src/cpp/grayplotDrawing/DrawabelGrayplotJavaMapper.cpp \
src/cpp/grayplotDrawing/DrawableGrayplotBridgeFactory.cpp \
src/cpp/grayplotDrawing/DrawableGrayplotJoGL.cpp \
src/cpp/grayplotDrawing/GrayplotDecomposer.cpp \
src/cpp/grayplotDrawing/MatplotDecomposer.cpp \
src/jni/DrawableAxesGL.cpp \
src/cpp/axesDrawing/AxesTicksComputer.cpp \
src/cpp/axesDrawing/ConcreteDrawableAxes.cpp \
src/cpp/axesDrawing/DrawableAxesBridgeFactory.cpp \
src/cpp/axesDrawing/DrawableAxesJavaMapper.cpp \
src/cpp/axesDrawing/DrawableAxesJoGL.cpp \
src/cpp/segsDrawing/DecomposeSegsStrategy.cpp \
src/cpp/axesDrawing/AxesPositioner.cpp \
src/cpp/axesDrawing/AxesTicksDrawerJoGL.cpp \
src/cpp/subwinDrawing/BottomXAxisPositioner.cpp \
src/cpp/subwinDrawing/LeftYAxisPositioner.cpp \
src/cpp/subwinDrawing/XAxisPositioner.cpp \
src/cpp/subwinDrawing/YAxisPositioner.cpp \
src/cpp/subwinDrawing/MiddleXAxisPositioner.cpp \
src/cpp/subwinDrawing/MiddleYAxisPositioner.cpp \
src/cpp/subwinDrawing/OriginXAxisPositioner.cpp \
src/cpp/subwinDrawing/OriginYAxisPositioner.cpp \
src/cpp/subwinDrawing/AxisPositioner.cpp \
src/cpp/subwinDrawing/TopXAxisPositioner.cpp \
src/cpp/subwinDrawing/ZAxisPositioner.cpp \
src/cpp/subwinDrawing/RightYAxisPositioner.cpp \
src/cpp/subwinDrawing/SubwinAxisPositioner.cpp \
src/cpp/subwinDrawing/TicksDrawerJavaMapper.cpp \
src/cpp/subwinDrawing/GridDrawerJavaMapper.cpp \
src/jni/TicksDrawerGL.cpp \
src/jni/GridDrawerGL.cpp \
src/cpp/RendererFontManager.cpp \
src/jni/XlFontManager.cpp \
src/cpp/subwinDrawing/AutoLogSubticksComputer.cpp \
src/cpp/subwinDrawing/AutomaticSubticksComputer.cpp \
src/cpp/subwinDrawing/UserDefLogSubticksComputer.cpp \
src/cpp/subwinDrawing/UserDefinedSubticksComputer.cpp \
src/cpp/axesDrawing/AxesSubticksComputer.cpp \
src/cpp/rendererBasicAlgos.cpp \
src/jni/SubwinBackgroundDrawerGL.cpp \
src/cpp/subwinDrawing/SubwinBackgroundDrawerJavaMapper.cpp \
src/cpp/subwinDrawing/SubwinBackgroundDrawerJoGL.cpp \
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
src/jni/DrawableAxesGL.giws.xml \
src/jni/DrawableFigureGL.giws.xml \
src/jni/DrawableGrayplotGL.giws.xml \
src/jni/DrawableSegsGL.giws.xml \
src/jni/DrawableSubwinGL.giws.xml \
src/jni/DrawableSurfaceGL.giws.xml \
src/jni/FullBoxDrawerGL.giws.xml \
src/jni/GraphicSynchronizerJava.giws.xml \
src/jni/HalfBoxDrawerGL.giws.xml \
src/jni/IsometricCameraGL.giws.xml \
src/jni/IsoViewCameraGL.giws.xml \
src/jni/SegsArrowDrawerGL.giws.xml \
src/jni/SegsLineDrawerGL.giws.xml \
src/jni/SegsMarkDrawerGL.giws.xml \
src/jni/SurfaceFacetDrawerGL.giws.xml \
src/jni/SurfaceLineDrawerGL.giws.xml \
src/jni/SurfaceMarkDrawerGL.giws.xml \
src/jni/TicksDrawerGL.giws.xml \
src/jni/GridDrawerGL.giws.xml \
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
