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
src/cpp/figureDrawing/DrawableFigureImpFactory.cpp \
src/cpp/figureDrawing/DrawableFigure.cpp \
src/cpp/figureDrawing/ConcreteDrawableFigure.cpp \
src/cpp/figureDrawing/DrawableFigureImp.cpp \
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
src/cpp/rectangleDrawing/DrawableRectangleImpFactory.cpp \
src/cpp/rectangleDrawing/DrawableRectangle.cpp \
src/cpp/rectangleDrawing/DrawableRectangleImp.cpp \
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
src/cpp/subwinDrawing/DrawableSubwinBridge.cpp \
src/cpp/subwinDrawing/Camera.cpp \
src/cpp/subwinDrawing/CameraBridgeFactory.cpp \
src/cpp/polylineDrawing/DrawablePolylineBrirdgeFactory.cpp \
src/cpp/polylineDrawing/DrawablePolylineJoGL.cpp \
src/cpp/polylineDrawing/DrawablePolylineBridge.cpp



# List of the gateway c files
GATEWAY_C_SOURCES = sci_gateway/c/gw_renderer.c

# Includes need for the compilation
libscirenderer_la_CPPFLAGS =  $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/cpp/ -I$(top_srcdir)/modules/jvm/includes -I$(top_srcdir)/modules/graphics/includes/ -I$(top_srcdir)/modules/graphics/includes/handleDrawing/ 

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = $(RENDERER_CPP_SOURCES) $(GATEWAY_C_SOURCES) 

## Libraries necessary
## Change to include other Scilab modules/lib
libscirenderer_la_LIBADD = $(top_builddir)/modules/graphics/libscigraphics.la

else

# All the sources needed by libscirenderer.la
libscirenderer_la_SOURCES = src/norenderer/norenderer.c

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
