##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### TEMPLATE MAKEFILE
### INRIA - Scilab 2007
##########

if GUI
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
src/cpp/rectangleDrawing/DrawableRectangleBridge.cpp \
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
src/cpp/GraphicSynchronization/GraphicSynchronizerJava.cpp \
src/jni/Object.cpp \
src/cpp/rectangleDrawing/ConcreteDrawableRectangle.cpp \
src/cpp/subwinDrawing/DrawableSubwinJavaMapper.cpp \
src/cpp/figureDrawing/DrawableFigureJavaMapper.cpp \
src/cpp/polylineDrawing/DrawablePolylineJavaMapper.cpp \
src/cpp/rectangleDrawing/DrawableRectangleJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleFillDrawerJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleLineDrawerJavaMapper.cpp \
src/cpp/rectangleDrawing/RectangleMarkDrawerJavaMapper.cpp \
src/jni/ScilabGraphicWindow.cpp \
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
src/cpp/textDrawing/DrawTextBoxStrategy.cpp \
src/cpp/textDrawing/TextLineBoxDrawerJoGL.cpp \
src/cpp/textDrawing/TextFillBoxDrawerJoGL.cpp \
src/jni/IsoViewCameraGL.cpp \
src/jni/IsometricCameraGL.cpp \
src/cpp/subwinDrawing/IsoViewCameraJavaMapper.cpp \
src/cpp/subwinDrawing/IsometricCameraJavaMapper.cpp

# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_renderer.c

# Includes need for the compilation
libscirenderer_la_CPPFLAGS =  	$(JAVA_JNI_INCLUDE) \
				-Iincludes/ \
				-Isrc/cpp/ \
				-Isrc/jni/ \
				-I$(top_srcdir)/modules/jvm/includes \
				-I$(top_srcdir)/modules/graphics/includes/ \
				-I$(top_srcdir)/modules/graphics/includes/handleDrawing/ \
				-I$(top_srcdir)/libs/doublylinkedlist/includes \
				-I$(top_srcdir)/modules/output_stream/includes

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = $(RENDERER_CPP_SOURCES) $(GATEWAY_C_SOURCES)

## Libraries necessary
## Change to include other Scilab modules/lib
#libscirenderer_la_LIBADD = $(top_builddir)/modules/graphics/libscigraphics.la

GIWS_WRAPPERS=src/jni/GraphicSynchronizer.giws.xml \
src/jni/PolylineBarDrawerGL.giws.xml \
src/jni/CameraGL.giws.xml \
src/jni/DrawablePolylineGL.giws.xml \
src/jni/PolylineArrrowDrawerGL.giws.xml \
src/jni/ScilabGraphicWindow.giws.xml \
src/jni/DrawableRectangleGL.giws.xml \
src/jni/DrawableArcGL.giws.xml \
src/jni/PolylineMarkdrawerGL.giws.xml \
src/jni/RectangleFillDrawerGL.giws.xml \
src/jni/ArcFillDrawerGL.giws.xml \
src/jni/PolylineInterpColorDrawerGL.giws.xml \
src/jni/PolylineLineDrawerGL.giws.xml \
src/jni/DrawableSubwinGL.giws.xml \
src/jni/RectangleLineDrawerGL.giws.xml \
src/jni/ArcLineDrawerGL.giws.xml \
src/jni/DrawableFigureGL.giws.xml \
src/jni/PolylineFillDrawer.giws.xml \
src/jni/RectangleMarkDrawerGL.giws.xml \
src/jni/DrawableTextGL.giws.xml \
src/jni/StandardTextDrawerGL.giws.xml


else

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = src/norenderer/norenderer.c

libscirenderer_la_CPPFLAGS = -I$(top_srcdir)/modules/output_stream/includes
endif

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscirenderer.la


#### Name of the module and the path ######
modulename=renderer

#### renderer : Conf files ####
libscirenderer_la_rootdir = $(mydatadir)
libscirenderer_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### renderer : init scripts ####
libscirenderer_la_etcdir = $(mydatadir)/etc
libscirenderer_la_etc_DATA = etc/renderer.quit etc/renderer.start

#### renderer : gateway declaration ####
#libscirenderer_la_sci_gatewaydir = $(mydatadir)/sci_gateway
#libscirenderer_la_sci_gateway_DATA = sci_gateway/renderer_gateway.xml

#### renderer : include files ####
libscirenderer_la_includedir=$(pkgincludedir)/renderer
libscirenderer_la_include_HEADERS = includes/gw_renderer.h

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1

USEANT=1

include $(top_srcdir)/Makefile.incl.am
