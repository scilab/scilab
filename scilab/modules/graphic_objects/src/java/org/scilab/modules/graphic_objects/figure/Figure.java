/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.figure;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ANTIALIASING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BACKGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSEREQUESTFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLORMAP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLORMAP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PIXMAP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZEFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ROTATION_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VIEWPORT__;
/**
 * Figure class
 * @author Manuel JULIACHS
 */
public class Figure extends GraphicObject {
    /** Figure properties names */
    private enum FigureProperty {
        INFOMESSAGE, COLORMAP, COLORMAPSIZE,
        BACKGROUND, TAG, ROTATIONTYPE, RESIZEFCN, CLOSEREQUESTFCN
    };

    /** Specifies whether rotation applies to a single subwindow or to all the figure's subwindows */
    public enum RotationType { UNARY, MULTIPLE;

                               /**
                                * Converts an integer to the corresponding enum
                                * @param intValue the integer value
                                * @return the rotation type enum
                                */
    public static RotationType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return RotationType.UNARY;
            case 1:
                return RotationType.MULTIPLE;
            default:
                return null;
        }
    }
                             }

    /** Pixel drawing logical operations */
    private enum PixelDrawingMode { CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR, NOR,
                                    EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET;

                                    /**
                                     * Converts an integer to the corresponding enum
                                     * @param intValue the integer value
                                     * @return the pixel drawing mode enum
                                     */
    public static PixelDrawingMode intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return PixelDrawingMode.CLEAR;
            case 1:
                return PixelDrawingMode.AND;
            case 2:
                return PixelDrawingMode.ANDREVERSE;
            case 3:
                return PixelDrawingMode.COPY;
            case 4:
                return PixelDrawingMode.ANDINVERTED;
            case 5:
                return PixelDrawingMode.NOOP;
            case 6:
                return PixelDrawingMode.XOR;
            case 7:
                return PixelDrawingMode.OR;
            case 8:
                return PixelDrawingMode.NOR;
            case 9:
                return PixelDrawingMode.EQUIV;
            case 10:
                return PixelDrawingMode.INVERT;
            case 11:
                return PixelDrawingMode.ORREVERSE;
            case 12:
                return PixelDrawingMode.COPYINVERTED;
            case 13:
                return PixelDrawingMode.ORINVERTED;
            case 14:
                return PixelDrawingMode.NAND;
            case 15:
                return PixelDrawingMode.SET;
            default:
                return null;
        }
    }
                                  };

    /** FigureDimensions properties names */
    public enum FigureDimensionsProperty { POSITION, SIZE };

    /**
     * FigureDimensions class
     */
    private class FigureDimensions {
        /** Position (x,y) of the upper-left corner in the top-level window */
        private int[] position;

        /** Dimensions (w, h) of the figure window */
        private int[] size;

        /**
         * Default constructor
         */
        public FigureDimensions() {
            position = new int[2];
            size = new int[2];
        }

        /**
         * Copy constructor
         * @param figureDimensions the FigureDimensions to copy
         */
        public FigureDimensions(FigureDimensions figureDimensions) {
            this.position = new int[2];
            this.position[0] = figureDimensions.position[0];
            this.position[1] = figureDimensions.position[1];

            this.size = new int[2];
            this.size[0] = figureDimensions.size[0];
            this.size[1] = figureDimensions.size[1];
        }
    }

    /** CanvasProperty properties names */
    public enum CanvasProperty { AUTORESIZE, VIEWPORT, AXESSIZE };

    /**
     * Canvas class
     */
    private class Canvas {
        /** Specifies whether automatic resizing is used */
        private boolean autoResize;

        /** Viewport position (x,y) */
        private int[] viewport;

        /** Rendering canvas (w,h) dimensions */
        private int[] axesSize;

        /**
         * Default constructor
         */
        public Canvas() {
            autoResize = false;
            viewport = new int[2];
            axesSize = new int[2];
        }

        /**
         * Copy constructor
         * @param canvas the Canvas to copy
         */
        public Canvas(Canvas canvas) {
            this.autoResize = canvas.autoResize;

            this.viewport = new int[2];

            this.viewport[0] = canvas.viewport[0];
            this.viewport[1] = canvas.viewport[1];

            this.axesSize = new int[2];

            this.axesSize[0] = canvas.axesSize[0];
            this.axesSize[1] = canvas.axesSize[1];
        }
    }

    /** FigureName properties names */
    public enum FigureNameProperty { NAME, ID };

    /**
     * FigureName class
     */
    public class FigureName implements Cloneable {
        /** Name */
        private String name;

        /** Identifier */
        private int id;

        public FigureName clone() throws CloneNotSupportedException {
            FigureName copy;

            //    return (FigureName) super.clone();

            copy = (FigureName) super.clone();

            copy.name = new String(this.name);
            copy.id = this.id;

            return copy;
        }

        /**
         * Default constructor
         */
        public FigureName() {
            name = "";
            id = 0;
        }

        /**
         * Copy constructor
         * @param figureName the FigureName to copy
         */
        public FigureName(FigureName figureName) {
            name = new String(figureName.name);
            id = figureName.id;
        }

    }

    /** RenderingMode properties names */
    public enum RenderingModeProperty { PIXMAP, PIXELDRAWINGMODE, ANTIALIASING, IMMEDIATEDRAWING };

    /**
     * RenderingMode class
     */
    private class RenderingMode {
        /** Specifies rendering into a pixmap */
        private boolean pixmap;

        /** Specifies the pixel drawing mode used */
        private PixelDrawingMode pixelDrawingMode;

        /** Antialising level (0 == off) */
        private int antialiasing;

        /** Specifies whether immediate drawing is used or not */
        private boolean immediateDrawing;

        /**
         * Default constructor
         */
        public RenderingMode() {
            pixmap = false;
            pixelDrawingMode = PixelDrawingMode.COPY;
            antialiasing = 0;
            immediateDrawing = true;
        }

        /**
         * Copy constructor
         * @param renderingMode the RenderingMode to copy
         */
        public RenderingMode(RenderingMode renderingMode) {
            pixmap = renderingMode.pixmap;
            pixelDrawingMode = renderingMode.pixelDrawingMode;
            antialiasing = renderingMode.antialiasing;
            immediateDrawing = renderingMode.immediateDrawing;
        }

    }

    /** EventHandler properties names */
    public enum EventHandlerProperty { EVENTHANDLER, EVENTHANDLERENABLE };

    /**
     * EventHandler class
     */
    private class EventHandler {
        /** Event handler string */
        private String eventHandler = "";

        /** Specifies whether the event handler is enabled or not */
        private Boolean eventHandlerEnabled = false;

        /**
         * Default constructor
         */
        public EventHandler() {
            eventHandler = "";
            eventHandlerEnabled = false;
        }

        /**
         * Copy constructor
         * @param eventHandler the EventHandler to copy
         */
        public EventHandler(EventHandler eventHandler) {
            this.eventHandler = eventHandler.eventHandler;
            this.eventHandlerEnabled = eventHandler.eventHandlerEnabled;
        }
    }

    /** Figure dimensions */
    private FigureDimensions dimensions;

    /** Canvas property */
    private Canvas canvas;

    /** Figure name */
    private FigureName figureName;

    /** Info message */
    private String infoMessage;

    /**
     * Default ColorMap: (3 x N) matrix, where N is the
     * number of colors and 3 the number of color channels
     */
    private ColorMap colorMap;

    /** Rendering mode */
    private RenderingMode renderingMode;

    /** Background color */
    private int background;

    /** Event handler */
    private EventHandler eventHandler;

    /** ResizeFcn */
    private String resizeFcn;

    /** CloseRequestFcn */
    private String closeRequestFcn;

    /** Tag */
    private String tag;

    /** Rotation type */
    private RotationType rotation;

    /** Constructor */
    public Figure() {
        super();
        dimensions = new FigureDimensions();
        canvas = new Canvas();
        figureName = new FigureName();
        infoMessage = null;
        colorMap = new ColorMap();

        renderingMode = new RenderingMode();
        background = 0;
        eventHandler = new EventHandler();
        resizeFcn = "";
        closeRequestFcn = "";
        tag = "";
        rotation = RotationType.UNARY;
    }

    @Override
    public Figure clone() {
        Figure copy = null;
        copy = (Figure) super.clone();

        copy.dimensions = new FigureDimensions(this.dimensions);
        copy.canvas = new Canvas(this.canvas);
        copy.figureName = new FigureName(this.figureName);
        copy.colorMap = new ColorMap(this.colorMap);
        copy.renderingMode = new RenderingMode(this.renderingMode);
        copy.eventHandler = new EventHandler(this.eventHandler);

        copy.setValid(true);

        return copy;
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_POSITION__ :
                return FigureDimensionsProperty.POSITION;
            case __GO_SIZE__ :
                return FigureDimensionsProperty.SIZE;
            case __GO_AUTORESIZE__ :
                return CanvasProperty.AUTORESIZE;
            case __GO_VIEWPORT__ :
                return CanvasProperty.VIEWPORT;
            case __GO_AXES_SIZE__ :
                return CanvasProperty.AXESSIZE;
            case __GO_NAME__ :
                return FigureNameProperty.NAME;
            case __GO_ID__ :
                return FigureNameProperty.ID;
            case __GO_INFO_MESSAGE__ :
                return FigureProperty.INFOMESSAGE;
            case __GO_COLORMAP__ :
                return FigureProperty.COLORMAP;
            case __GO_COLORMAP_SIZE__ :
                return FigureProperty.COLORMAPSIZE;
            case __GO_PIXMAP__ :
                return RenderingModeProperty.PIXMAP;
            case __GO_PIXEL_DRAWING_MODE__ :
                return RenderingModeProperty.PIXELDRAWINGMODE;
            case __GO_ANTIALIASING__ :
                return RenderingModeProperty.ANTIALIASING;
            case __GO_IMMEDIATE_DRAWING__ :
                return RenderingModeProperty.IMMEDIATEDRAWING;
            case __GO_BACKGROUND__ :
                return FigureProperty.BACKGROUND;
            case __GO_EVENTHANDLER_NAME__ :
                return EventHandlerProperty.EVENTHANDLER;
            case __GO_EVENTHANDLER_ENABLE__ :
                return EventHandlerProperty.EVENTHANDLERENABLE;
            case __GO_TAG__ :
                return FigureProperty.TAG;
            case __GO_ROTATION_TYPE__ :
                return FigureProperty.ROTATIONTYPE;
            case __GO_RESIZEFCN__ :
                return FigureProperty.RESIZEFCN;
            case __GO_CLOSEREQUESTFCN__ :
                return FigureProperty.CLOSEREQUESTFCN;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == FigureDimensionsProperty.POSITION) {
            return getPosition();
        } else if (property == FigureDimensionsProperty.SIZE) {
            return getSize();
        } else if (property == CanvasProperty.AUTORESIZE) {
            return getAutoResize();
        } else if (property == CanvasProperty.VIEWPORT) {
            return getViewport();
        } else if (property == CanvasProperty.AXESSIZE) {
            return getAxesSize();
        } else if (property == FigureNameProperty.NAME) {
            return getName();
        } else if (property == FigureNameProperty.ID) {
            return getId();
        } else if (property == FigureProperty.INFOMESSAGE) {
            return getInfoMessage();
        } else if (property == FigureProperty.COLORMAP) {
            return getColorMap().getData();
        } else if (property == FigureProperty.COLORMAPSIZE) {
            return getColorMap().getSize();
        } else if (property == RenderingModeProperty.PIXMAP) {
            return getPixmap();
        } else if (property == RenderingModeProperty.PIXELDRAWINGMODE) {
            return getPixelDrawingMode();
        } else if (property == RenderingModeProperty.ANTIALIASING) {
            return getAntialiasing();
        } else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
            return getImmediateDrawing();
        } else if (property == FigureProperty.BACKGROUND) {
            return getBackground();
        } else if (property == EventHandlerProperty.EVENTHANDLER) {
            return getEventHandlerString();
        } else if (property == EventHandlerProperty.EVENTHANDLERENABLE) {
            return getEventHandlerEnable();
        } else if (property == FigureProperty.TAG) {
            return getTag();
        } else if (property == FigureProperty.ROTATIONTYPE) {
            return getRotation();
        } else if (property == FigureProperty.RESIZEFCN) {
            return getResizeFcn();
        } else if (property == FigureProperty.CLOSEREQUESTFCN) {
            return getCloseRequestFcn();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == FigureDimensionsProperty.POSITION) {
            setPosition((Integer[]) value);
        } else if (property == FigureDimensionsProperty.SIZE) {
            setSize((Integer[]) value);
        } else if (property == CanvasProperty.AUTORESIZE) {
            setAutoResize((Boolean) value);
        } else if (property == CanvasProperty.VIEWPORT) {
            setViewport((Integer[]) value);
        } else if (property == CanvasProperty.AXESSIZE) {
            setAxesSize((Integer[]) value);
        } else if (property == FigureNameProperty.NAME) {
            setName((String) value);
        } else if (property == FigureNameProperty.ID) {
            setId((Integer) value);
        } else if (property == FigureProperty.INFOMESSAGE) {
            setInfoMessage((String) value);
        } else if (property == FigureProperty.COLORMAP) {
            return getColorMap().setData((Double[]) value);
        } else if (property == RenderingModeProperty.PIXMAP) {
            setPixmap((Boolean) value);
        } else if (property == RenderingModeProperty.PIXELDRAWINGMODE) {
            setPixelDrawingMode((Integer) value);
        } else if (property == RenderingModeProperty.ANTIALIASING) {
            setAntialiasing((Integer) value);
        } else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
            return setImmediateDrawing((Boolean) value);
        } else if (property == FigureProperty.BACKGROUND) {
            setBackground((Integer) value);
        } else if (property == EventHandlerProperty.EVENTHANDLER) {
            setEventHandlerString((String) value);
        } else if (property == EventHandlerProperty.EVENTHANDLERENABLE) {
            setEventHandlerEnable((Boolean) value);
        } else if (property == FigureProperty.TAG) {
            setTag((String) value);
        } else if (property == FigureProperty.ROTATIONTYPE) {
            setRotation((Integer) value);
        } else if (property == FigureProperty.RESIZEFCN) {
            setResizeFcn((String) value);
        } else if (property == FigureProperty.CLOSEREQUESTFCN) {
            setCloseRequestFcn((String) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the background
     */
    public Integer getBackground() {
        return background;
    }

    /**
     * @param background the background to set
     */
    public void setBackground(Integer background) {
        this.background = background;
    }

    /**
     * @return the canvas
     */
    public Canvas getCanvas() {
        return canvas;
    }

    /**
     * @param canvas the canvas to set
     */
    public void setCanvas(Canvas canvas) {
        this.canvas = canvas;
    }

    /**
     * @return the autoresize
     */
    public Boolean getAutoResize() {
        return canvas.autoResize;
    }

    /**
     * @param autoResize the autoresize to set
     */
    public void setAutoResize(Boolean autoResize) {
        canvas.autoResize = autoResize;
    }

    /**
     * @return the viewport
     */
    public Integer[] getViewport() {
        Integer[] retViewport = new Integer[2];

        retViewport[0] = canvas.viewport[0];
        retViewport[1] = canvas.viewport[1];

        return retViewport;
    }

    /**
     * @param viewport the viewport to set
     */
    public void setViewport(Integer[] viewport) {
        canvas.viewport[0] = viewport[0];
        canvas.viewport[1] = viewport[1];
    }

    /**
     * @return the axes size
     */
    public Integer[] getAxesSize() {
        Integer[] retAxesSize = new Integer[2];

        retAxesSize[0] = canvas.axesSize[0];
        retAxesSize[1] = canvas.axesSize[1];

        return retAxesSize;
    }

    /**
     * @param axesSize the axes size to set
     */
    public void setAxesSize(Integer[] axesSize) {
        canvas.axesSize[0] = axesSize[0];
        canvas.axesSize[1] = axesSize[1];
    }

    /**
     * @return the colorMap
     */
    public ColorMap getColorMap() {
        return colorMap;
    }

    /**
     * @return the dimensions
     */
    public FigureDimensions getDimensions() {
        return dimensions;
    }

    /**
     * @param dimensions the dimensions to set
     */
    public void setDimensions(FigureDimensions dimensions) {
        this.dimensions = dimensions;
    }

    /**
     * @return the figure position
     */
    public Integer[] getPosition() {
        Integer[] retPosition = new Integer[2];

        retPosition[0] = dimensions.position[0];
        retPosition[1] = dimensions.position[1];

        return retPosition;
    }

    /**
     * @param position the position to set
     */
    public void setPosition(Integer[] position) {
        dimensions.position[0] = position[0];
        dimensions.position[1] = position[1];
    }

    /**
     * @return the figure size
     */
    public Integer[] getSize() {
        Integer[] retSize = new Integer[2];

        retSize[0] = dimensions.size[0];
        retSize[1] = dimensions.size[1];

        return retSize;
    }

    /**
     * @param size the size to set
     */
    public void setSize(Integer[] size) {
        dimensions.size[0] = size[0];
        dimensions.size[1] = size[1];
    }

    /**
     * @return the eventHandler
     */
    public EventHandler getEventHandler() {
        return eventHandler;
    }

    /**
     * @param eventHandler the eventHandler to set
     */
    public void setEventHandler(EventHandler eventHandler) {
        this.eventHandler = eventHandler;
    }

    /**
     * @return the eventHandler string
     */
    public String getEventHandlerString() {
        return eventHandler.eventHandler;
    }

    /**
     * @param eventHandlerString the eventHandler string to set
     */
    public void setEventHandlerString(String eventHandlerString) {
        eventHandler.eventHandler = eventHandlerString;
    }

    /**
     * @return the eventHandlerEnabled
     */
    public Boolean getEventHandlerEnable() {
        return eventHandler.eventHandlerEnabled;
    }

    /**
     * @param eventHandlerEnabled the eventHandlerEnabled to set
     */
    public void setEventHandlerEnable(Boolean eventHandlerEnabled) {
        eventHandler.eventHandlerEnabled = eventHandlerEnabled;
    }

    /**
     * @return the infoMessage
     */
    public String getInfoMessage() {
        return infoMessage;
    }

    /**
     * @param infoMessage the infoMessage to set
     */
    public void setInfoMessage(String infoMessage) {
        this.infoMessage = infoMessage;
    }

    /**
     * @return the name
     */
    public FigureName getFigureName() {
        return figureName;
    }

    /**
     * @param figureName the figure name to set
     */
    public void setFigureName(FigureName figureName) {
        this.figureName = figureName;
    }

    /**
     * @return the figure name
     */
    public String getName() {
        return figureName.name;
    }

    /**
     * @param name the name to set
     */
    public void setName(String name) {
        figureName.name = name;
    }

    /**
     * @return the id
     */
    public Integer getId() {
        return figureName.id;
    }

    /**
     * @param id the id to set
     */
    public void setId(Integer id) {
        figureName.id = id;
    }

    /**
     * @return the renderingMode
     */
    public RenderingMode getRenderingMode() {
        return renderingMode;
    }

    /**
     * @param renderingMode the renderingMode to set
     */
    public void setRenderingMode(RenderingMode renderingMode) {
        this.renderingMode = renderingMode;
    }

    /**
     * @return the pixmap
     */
    public Boolean getPixmap() {
        return renderingMode.pixmap;
    }

    /**
     * @param pixmap the pixmap to set
     */
    public void setPixmap(Boolean pixmap) {
        renderingMode.pixmap = pixmap;
    }

    /**
     * @return the pixel drawing mode enum
     */
    public PixelDrawingMode getPixelDrawingModeAsEnum() {
        return renderingMode.pixelDrawingMode;
    }

    /**
     * @return the pixel drawing mode
     */
    public Integer getPixelDrawingMode() {
        return getPixelDrawingModeAsEnum().ordinal();
    }

    /**
     * @param pixelDrawingMode the pixel drawing mode to set
     */
    public void setPixelDrawingMode(PixelDrawingMode pixelDrawingMode) {
        renderingMode.pixelDrawingMode = pixelDrawingMode;
    }

    /**
     * @param pixelDrawingMode the pixel drawing mode to set
     */
    public void setPixelDrawingMode(Integer pixelDrawingMode) {
        renderingMode.pixelDrawingMode = PixelDrawingMode.intToEnum(pixelDrawingMode);
    }

    /**
     * @return the pixmap
     */
    public Integer getAntialiasing() {
        return renderingMode.antialiasing;
    }

    /**
     * @param antialiasing the antialiasing to set
     */
    public void setAntialiasing(Integer antialiasing) {
        renderingMode.antialiasing = antialiasing;
    }

    /**
     * @return the immediateDrawing
     */
    public Boolean getImmediateDrawing() {
        return renderingMode.immediateDrawing;
    }

    /**
     * @param immediateDrawing the immediateDrawing to set
     */
    public UpdateStatus setImmediateDrawing(Boolean immediateDrawing) {
        if (renderingMode.immediateDrawing != immediateDrawing) {
            renderingMode.immediateDrawing = immediateDrawing;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the rotation
     */
    public RotationType getRotationAsEnum() {
        return rotation;
    }

    /**
     * @return the rotation
     */
    public Integer getRotation() {
        return getRotationAsEnum().ordinal();
    }

    /**
     * @param rotation the rotation to set
     */
    public void setRotation(RotationType rotation) {
        this.rotation = rotation;
    }

    /**
     * @param rotation the rotation to set
     */
    public void setRotation(Integer rotation) {
        RotationType rotationType = RotationType.intToEnum(rotation);
        if (rotationType != null) {
            this.rotation = rotationType;
        }
    }

    /**
     * @return the resize function
     */
    public String getResizeFcn() {
        return resizeFcn;
    }

    /**
     * @param resizeFcn the resize function to set
     */
    public void setResizeFcn(String resizeFcn) {
        this.resizeFcn = resizeFcn;
    }

    /**
     * @return the close request function
     */
    public String getCloseRequestFcn() {
        return closeRequestFcn;
    }

    /**
     * @param closeRequestFcn the close request function to set
     */
    public void setCloseRequestFcn(String closeRequestFcn) {
        this.closeRequestFcn = closeRequestFcn;
    }

    /**
     * @return the tag
     */
    public String getTag() {
        return tag;
    }

    /**
     * @param tag the tag to set
     */
    public void setTag(String tag) {
        this.tag = tag;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_FIGURE__;
    }

}
