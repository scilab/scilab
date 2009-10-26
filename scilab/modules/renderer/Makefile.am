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
libscirenderer_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### renderer : init scripts ####
libscirenderer_la_etcdir = $(mydatadir)/etc
libscirenderer_la_etc_DATA = etc/renderer.quit etc/renderer.start



# List of the c files
RENDERER_CPP_SOURCES = \
src/cpp/subwinDrawing/DrawableSubwinFactory.cpp \
src/cpp/subwinDrawing/DrawableSubwin.cpp \
src/cpp/figureDrawing/DrawableFigureJoGL.cpp \
src/cpp/figureDrawing/DrawableFigureBridgeFactory.cpp \
src/cpp/figureDrawing/DrawableFigure.cpp \
src/cpp/figureDrawing/ConcreteDrawableFigure.cpp \
src/cpp/figureDrawing/DrawableFigureFactory.cpp \
src/cpp/figureDrawing/FigureScilabCall.cpp \
src/cpp/compoundDrawing/DrawableCompoundFactory.cpp \
src/cpp/compoundDrawing/DrawableCompound.cpp \
src/cpp/textDrawing/DrawableTextFactory.cpp \
src/cpp/textDrawing/DrawableText.cpp \
src/cpp/polylineDrawing/DrawablePolylineFactory.cpp \
src/cpp/polylineDrawing/DrawablePolyline.cpp \
src/cpp/legendDrawing/DrawableLegend.cpp \
src/cpp/legendDrawing/DrawableLegendFactory.cpp \
src/cpp/segsDrawing/DrawableSegs.cpp \
src/cpp/segsDrawing/DrawableSegsFactory.cpp \
src/cpp/rectangleDrawing/DrawableRectangleFactory.cpp \
src/cpp/rectangleDrawing/RectangleMarkDrawerJoGL.cpp \
src/cpp/rectangleDrawing/DrawableRectangleBridgeFactory.cpp \
src/cpp/rectangleDrawing/DrawableRectangle.cpp \
src/cpp/rectangleDrawing/DrawableRectangleJoGL.cpp \
src/cpp/rectangleDrawing/RectangleFillDrawerJoGL.cpp \
src/cpp/rectangleDrawing/RectangleLineDrawerJoGL.cpp \
src/cpp/arcDrawing/DrawableArcFactory.cpp \
src/cpp/arcDrawing/DrawableArc.cpp \
src/cpp/labelDrawing/DrawableLabel.cpp \
src/cpp/labelDrawing/DrawableLabelFactory.cpp \
src/cpp/surfaceDrawing/DrawableSurfaceFactory.cpp \
src/cpp/surfaceDrawing/DrawableSurface.cpp \
src/cpp/fecDrawing/DrawableFec.cpp \
src/cpp/fecDrawing/DrawableFecFactory.cpp \
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
src/cpp/polylineDrawing/DrawablePolylineBrirdgeFactory.cpp \
src/cpp/polylineDrawing/DrawablePolylineJoGL.cpp \
src/cpp/DrawableClippedObjectJoGL.cpp \
src/cpp/GraphicSynchronization/GlobalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerBridgeFactory.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerFactory.cpp \
src/cpp/GraphicSynchronization/LocalSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizer.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerInterface.cpp \
src/cpp/GraphicSynchronization/GraphicSynchronizerJavaMapper.cpp \
src/cpp/rectangleDrawing/ConcreteDrawableRectangle.cpp \
src/cpp/subwinDrawing/DrawableSubwinJavaMapper.cpp \
src/cpp/figureDrawing/DrawableFigureJavaMapper.cpp \
src/cpp/polylineDrawing/DrawablePolylineJavaMapper.cpp \
src/cpp/rectangleDrawing/DrawableRectangleJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleFillDrawerJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleLineDrawerJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleMarkDrawerJavaMapper.cpp \
src/jni/DrawableFigureGL.cpp \
src/jni/DrawableSubwinGL.cpp \
src/jni/DrawablePolylineGL.cpp \
src/jni/DrawableRectangleGL.cpp \
src/jni/RectangleFillDrawerGL.cpp \
src/jni/RectangleLineDrawerGL.cpp \
src/jni/RectangleMarkDrawerGL.cpp \
src/cpp/arcDrawing/ArcLineDrawerJoGL.cpp \
src/cpp/arcDrawing/DrawableArcBridgeFactory.cpp \
src/cpp/arcDrawing/DrawableArcJavaMapper.cpp \
src/cpp/arcDrawing/DrawableArcJoGL.cpp \
src/cpp/arcDrawing/ArcFillDrawerJavaMapper.cpp \
src/cpp/arcDrawing/ArcLineDrawerJavaMapper.cpp \
src/cpp/arcDrawing/ConcreteDrawableArc.cpp \
src/cpp/arcDrawing/ArcFillDrawerJoGL.cpp \
src/jni/ArcLineDrawerGL.cpp \
src/jni/ArcFillDrawerGL.cpp \
src/jni/DrawableArcGL.cpp \
src/jni/FigureScilabCall_wrap.c \
src/cpp/polylineDrawing/PolylineLineDrawerJavaMapper.cpp \
src/cpp/polylineDrawing/InterpolatedDecomposition.cpp \
src/cpp/polylineDrawing/StairCaseDecomposition.cpp \
src/cpp/polylineDrawing/ConcreteDrawablePolyline.cpp \
src/cpp/polylineDrawing/PolylineLineDrawerJoGL.cpp \
src/jni/PolylineLineDrawerGL.cpp \
src/cpp/polylineDrawing/PolylineFillDrawerJavaMapper.cpp \
src/cpp/polylineDrawing/PolylineFillDrawerJoGL.cpp \
src/jni/PolylineFillDrawerGL.cpp \
src/cpp/polylineDrawing/PolylineMarkDrawerJavaMapper.cpp \
src/cpp/polylineDrawing/PolylineMarkDrawerJoGL.cpp \
src/jni/PolylineMarkDrawerGL.cpp \
src/cpp/polylineDrawing/PolylineArrowDrawerJavaMapper.cpp \
src/cpp/polylineDrawing/PolylineArrowDrawerJoGL.cpp \
src/jni/PolylineArrowDrawerGL.cpp \
src/cpp/polylineDrawing/PolylineBarDrawerJoGL.cpp \
src/cpp/polylineDrawing/BarDecomposition.cpp \
src/cpp/polylineDrawing/PolylineBarDrawerJavaMapper.cpp \
src/jni/PolylineBarDrawerGL.cpp \
src/cpp/polylineDrawing/PolylineInterpColorDrawerJavaMapper.cpp \
src/cpp/polylineDrawing/PolylineInterpColorDrawerJoGL.cpp \
src/jni/PolylineInterpColorDrawerGL.cpp \
src/cpp/textDrawing/DrawableTextJoGL.cpp \
src/cpp/textDrawing/DrawableTextBridgeFactory.cpp \
src/cpp/textDrawing/DrawableTextJavaMapper.cpp \
src/jni/DrawableTextGL.cpp \
src/cpp/textDrawing/ConcreteDrawableText.cpp \
src/cpp/textDrawing/StandardTextDrawerJavaMapper.cpp \
src/cpp/textDrawing/StandardTextDrawerJoGL.cpp \
src/jni/StandardTextDrawerGL.cpp \
src/cpp/textDrawing/TextContentDrawerJoGL.cpp \
src/cpp/textDrawing/FilledTextDrawerJavaMapper.cpp \
src/cpp/textDrawing/FilledTextDrawerJoGL.cpp \
src/jni/FilledTextDrawerGL.cpp \
src/cpp/textDrawing/CenteredTextDrawerJavaMapper.cpp \
src/cpp/textDrawing/CenteredTextDrawerJoGL.cpp \
src/jni/CenteredTextDrawerGL.cpp \
src/jni/IsoViewCameraGL.cpp \
src/jni/IsometricCameraGL.cpp \
src/cpp/subwinDrawing/IsoViewCameraJavaMapper.cpp \
src/cpp/subwinDrawing/IsometricCameraJavaMapper.cpp \
src/cpp/subwinDrawing/LinearBoundsComputer.cpp \
src/cpp/subwinDrawing/ConcreteDrawableSubwin.cpp \
src/cpp/subwinDrawing/LogarithmicBoundsComputer.cpp \
src/cpp/polylineDrawing/VerticalBarDecomposition.cpp \
src/cpp/polylineDrawing/HorizontalBarDecomposition.cpp \
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
src/cpp/labelDrawing/LabelPositioner.cpp \
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
src/jni/DrawableFecGL.cpp \
src/jni/FecLineDrawerGL.cpp \
src/jni/FecFacetDrawerGL.cpp \
src/cpp/fecDrawing/ConcreteDrawableFec.cpp \
src/cpp/fecDrawing/DrawableFecBridgeFactory.cpp \
src/cpp/fecDrawing/DrawableFecJavaMapper.cpp \
src/cpp/fecDrawing/DrawableFecJoGL.cpp \
src/cpp/fecDrawing/FecFacetDrawerJavaMapper.cpp \
src/cpp/fecDrawing/FecFacetDrawerJoGL.cpp \
src/cpp/fecDrawing/FecLineDrawerJavaMapper.cpp \
src/cpp/fecDrawing/FecLineDrawerJoGL.cpp \
src/jni/DrawableAxesGL.cpp \
src/cpp/legendDrawing/ConcreteDrawableLegend.cpp \
src/cpp/axesDrawing/AxesTicksComputer.cpp \
src/cpp/axesDrawing/ConcreteDrawableAxes.cpp \
src/cpp/axesDrawing/DrawableAxesBridgeFactory.cpp \
src/cpp/axesDrawing/DrawableAxesJavaMapper.cpp \
src/cpp/axesDrawing/DrawableAxesJoGL.cpp \
src/cpp/segsDrawing/DecomposeSegsStrategy.cpp \
src/cpp/labelDrawing/TitlePositioner.cpp \
src/cpp/labelDrawing/XLabelPositioner.cpp \
src/cpp/labelDrawing/YLabelPositioner.cpp \
src/cpp/labelDrawing/ZLabelPositioner.cpp \
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
src/jni/RenderingChecker.cpp \
src/cpp/RendererFontManager.cpp \
src/jni/XlFontManager.cpp \
src/cpp/subwinDrawing/AutoLogSubticksComputer.cpp \
src/cpp/subwinDrawing/AutomaticSubticksComputer.cpp \
src/cpp/subwinDrawing/UserDefLogSubticksComputer.cpp \
src/cpp/subwinDrawing/UserDefinedSubticksComputer.cpp \
src/cpp/axesDrawing/AxesSubticksComputer.cpp \
src/jni/GiwsException.cpp \
src/cpp/rendererBasicAlgos.cpp \
src/jni/SubwinBackgroundDrawerGL.cpp \
src/cpp/subwinDrawing/SubwinBackgroundDrawerJavaMapper.cpp \
src/cpp/subwinDrawing/SubwinBackgroundDrawerJoGL.cpp

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_renderer.c

# Includes need for the compilation
libscirenderer_la_CPPFLAGS =  	$(JAVA_JNI_INCLUDE) \
				-I$(srcdir)/includes/ \
				-I$(srcdir)/src/cpp/ \
				-I$(srcdir)/src/jni/ \
				-I$(top_srcdir)/modules/jvm/includes \
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
libscirenderer_la_SOURCES = $(RENDERER_CPP_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscirenderer_la_CFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
# cyclic $(top_builddir)/modules/graphics/libscigraphics.la 
libscirenderer_la_LIBADD =  $(top_builddir)/libs/doublylinkedlist/libscidoublylinkedlist.la $(top_builddir)/modules/output_stream/libscioutput_stream.la

# needs the rendererer before $(top_builddir)/modules/jvm/libscijvm.la


GIWS_WRAPPERS=src/jni/ArcFillDrawerGL.giws.xml \
src/jni/ArcLineDrawerGL.giws.xml \
src/jni/BackTrihedronDrawerGL.giws.xml \
src/jni/CenteredTextDrawerGL.giws.xml \
src/jni/DrawableArcGL.giws.xml \
src/jni/DrawableAxesGL.giws.xml \
src/jni/DrawableFecGL.giws.xml \
src/jni/DrawableFigureGL.giws.xml \
src/jni/DrawableGrayplotGL.giws.xml \
src/jni/DrawablePolylineGL.giws.xml \
src/jni/DrawableRectangleGL.giws.xml \
src/jni/DrawableSegsGL.giws.xml \
src/jni/DrawableSubwinGL.giws.xml \
src/jni/DrawableSurfaceGL.giws.xml \
src/jni/DrawableTextGL.giws.xml \
src/jni/FecFacetDrawerGL.giws.xml \
src/jni/FecLineDrawerGL.giws.xml \
src/jni/FilledTextDrawerGL.giws.xml \
src/jni/FullBoxDrawerGL.giws.xml \
src/jni/GraphicSynchronizerJava.giws.xml \
src/jni/HalfBoxDrawerGL.giws.xml \
src/jni/IsometricCameraGL.giws.xml \
src/jni/IsoViewCameraGL.giws.xml \
src/jni/PolylineArrrowDrawerGL.giws.xml \
src/jni/PolylineBarDrawerGL.giws.xml \
src/jni/PolylineFillDrawer.giws.xml \
src/jni/PolylineInterpColorDrawerGL.giws.xml \
src/jni/PolylineLineDrawerGL.giws.xml \
src/jni/PolylineMarkdrawerGL.giws.xml \
src/jni/RectangleFillDrawerGL.giws.xml \
src/jni/RectangleLineDrawerGL.giws.xml \
src/jni/RectangleMarkDrawerGL.giws.xml \
src/jni/SegsArrowDrawerGL.giws.xml \
src/jni/SegsLineDrawerGL.giws.xml \
src/jni/SegsMarkDrawerGL.giws.xml \
src/jni/StandardTextDrawerGL.giws.xml \
src/jni/SurfaceFacetDrawerGL.giws.xml \
src/jni/SurfaceLineDrawerGL.giws.xml \
src/jni/SurfaceMarkDrawerGL.giws.xml \
src/jni/TicksDrawerGL.giws.xml \
src/jni/GridDrawerGL.giws.xml \
src/jni/RenderingChecker.giws.xml \
src/jni/XlFontManager.giws.xml \
src/jni/SubwinBackgroundDrawerGL.giws.xml

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1


if GIWS
BUILT_SOURCES=giws
endif

# All the sources needed by libscirenderer.la
libscirenderer_disable_la_SOURCES = src/norenderer/norenderer.c

libscirenderer_disable_la_CPPFLAGS = -I$(top_srcdir)/modules/output_stream/includes
libscirenderer_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES =  libscirenderer-disable.la

if GUI
USEANT=1
pkglib_LTLIBRARIES += libscirenderer.la
endif

libscirenderer_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

#### renderer : gateway declaration ####
#libscirenderer_la_sci_gatewaydir = $(mydatadir)/sci_gateway
#libscirenderer_la_sci_gateway_DATA = sci_gateway/renderer_gateway.xml

include $(top_srcdir)/Makefile.incl.am
