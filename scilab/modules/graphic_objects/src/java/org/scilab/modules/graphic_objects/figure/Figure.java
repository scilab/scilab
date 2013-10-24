/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.figure;

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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VIEWPORT__;

import java.util.Arrays;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.Antialiasing;
import org.scilab.modules.graphic_objects.utils.PixelDrawingMode;
import org.scilab.modules.graphic_objects.utils.RotationType;
/**
 * Figure class
 * @author Manuel JULIACHS
 */
public class Figure extends GraphicObject {
    /** Figure properties names */
    private enum FigureProperty {
        INFOMESSAGE, COLORMAP, COLORMAPSIZE,
        BACKGROUND, ROTATIONTYPE, RESIZEFCN, CLOSEREQUESTFCN
    };

    /** FigureDimensions properties names */
    public enum FigureDimensionsProperty { POSITION, SIZE };

    /**
     * FigureDimensions class
     */
    private class FigureDimensions {
        /** Position (x,y) of the upper-left corner in the top-level window */
        private Integer[] position;

        /** Dimensions (w, h) of the figure window */
        private Integer[] size;

        /**
         * Default constructor
         */
        public FigureDimensions() {
            position = new Integer[2];
            size = new Integer[2];
        }

        /**
         * Copy constructor
         * @param figureDimensions the FigureDimensions to copy
         */
        public FigureDimensions(FigureDimensions figureDimensions) {
            this.position = new Integer[2];
            this.position[0] = figureDimensions.position[0];
            this.position[1] = figureDimensions.position[1];

            this.size = new Integer[2];
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
        private Integer[] viewport;

        /** Rendering canvas (w,h) dimensions */
        private Integer[] axesSize;

        /**
         * Default constructor
         */
        public Canvas() {
            autoResize = false;
            viewport = new Integer[] {0, 0};
            axesSize = new Integer[] {0, 0};
        }

        /**
         * Copy constructor
         * @param canvas the Canvas to copy
         */
        public Canvas(Canvas canvas) {
            this.autoResize = canvas.autoResize;

            this.viewport = new Integer[2];

            this.viewport[0] = canvas.viewport[0];
            this.viewport[1] = canvas.viewport[1];

            this.axesSize = new Integer[2];

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
        private Antialiasing antialiasing;

        /** Specifies whether immediate drawing is used or not */
        private boolean immediateDrawing;

        /**
         * Default constructor
         */
        public RenderingMode() {
            pixmap = false;
            pixelDrawingMode = PixelDrawingMode.COPY;
            antialiasing = Antialiasing.OFF;
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
            return getPixelDrawingModeAsInteger();
        } else if (property == RenderingModeProperty.ANTIALIASING) {
            return getAntialiasing();
        } else if (property == RenderingModeProperty.IMMEDIATEDRAWING) {
            return getImmediateDrawing();
        } else if (property == FigureProperty.BACKGROUND) {
            return getBackground();
        } else if (property == EventHandlerProperty.EVENTHANDLER) {
            return getEventHandler();
        } else if (property == EventHandlerProperty.EVENTHANDLERENABLE) {
            return getEventHandlerEnable();
        } else if (property == FigureProperty.ROTATIONTYPE) {
            return getRotationStyleAsInteger();
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
        if (property instanceof FigureProperty) {
            switch ((FigureProperty)property) {
                case BACKGROUND:
                    return setBackground((Integer) value);
                case CLOSEREQUESTFCN:
                    return setCloseRequestFcn((String) value);
                case COLORMAP:
                    return getColorMap().setData((Double[]) value);
                case COLORMAPSIZE:
                    return UpdateStatus.NoChange;
                case INFOMESSAGE:
                    return setInfoMessage((String) value);
                case RESIZEFCN:
                    return setResizeFcn((String) value);
                case ROTATIONTYPE:
                    return setRotationStyle((Integer) value);
            }
        } else if (property instanceof CanvasProperty) {
            switch ((CanvasProperty)property) {
                case AUTORESIZE:
                    return setAutoResize((Boolean) value);
                case AXESSIZE:
                    return setAxesSize((Integer[]) value);
                case VIEWPORT:
                    return setViewport((Integer[]) value);
            }
        } else if (property instanceof FigureDimensionsProperty) {
            switch ((FigureDimensionsProperty)property) {
                case POSITION:
                    return setPosition((Integer[]) value);
                case SIZE:
                    return setSize((Integer[]) value);
            }
        } else if (property instanceof FigureNameProperty) {
            switch ((FigureNameProperty)property) {
                case ID:
                    return setId((Integer) value);
                case NAME:
                    return setName((String) value);
            }
        } else if (property instanceof RenderingModeProperty) {
            switch ((RenderingModeProperty)property) {
                case ANTIALIASING:
                    return setAntialiasing((Antialiasing) value);
                case IMMEDIATEDRAWING:
                    return setImmediateDrawing((Boolean) value);
                case PIXELDRAWINGMODE:
                    return setPixelDrawingMode((Integer) value);
                case PIXMAP:
                    return setPixmap((Boolean) value);
            }
        } else if (property instanceof EventHandlerProperty) {
            switch ((EventHandlerProperty)property) {
                case EVENTHANDLER:
                    return setEventHandler((String) value);
                case EVENTHANDLERENABLE:
                    return setEventHandlerEnable((Boolean) value);
            }
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
    public UpdateStatus setBackground(int background) {
        if (this.background == background) {
            return UpdateStatus.NoChange;
        }
        this.background = background;
        return UpdateStatus.Success;
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
    public UpdateStatus setCanvas(Canvas canvas) {
        this.canvas = canvas;
        return UpdateStatus.Success;
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
    public UpdateStatus setAutoResize(boolean autoResize) {
        if (canvas.autoResize == autoResize) {
            return UpdateStatus.NoChange;
        }
        canvas.autoResize = autoResize;
        return UpdateStatus.Success;
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
    public UpdateStatus setViewport(Integer[] viewport) {
        if (Arrays.equals(canvas.viewport, viewport)) {
            return UpdateStatus.NoChange;
        }
        canvas.viewport[0] = viewport[0];
        canvas.viewport[1] = viewport[1];
        return UpdateStatus.Success;
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
    public UpdateStatus setAxesSize(Integer[] axesSize) {
        if (Arrays.equals(canvas.axesSize, axesSize)) {
            //must return Success to broadcast information
            return UpdateStatus.Success;
        }
        canvas.axesSize[0] = axesSize[0];
        canvas.axesSize[1] = axesSize[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the colorMap
     */
    public ColorMap getColorMap() {
        return colorMap;
    }

    public UpdateStatus setColorMap(Double[] colorMap) {
        return getColorMap().setData(colorMap);
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
    public UpdateStatus setDimensions(FigureDimensions dimensions) {
        if (this.dimensions == dimensions) {
            return UpdateStatus.NoChange;
        }
        this.dimensions = dimensions;
        return UpdateStatus.Success;
    }

    /**
     * @return the figure position
     */
    public Integer[] getFigurePosition() {
        return getPosition();
    }

    public Integer[] getPosition() {
        Integer[] retPosition = new Integer[2];

        retPosition[0] = dimensions.position[0];
        retPosition[1] = dimensions.position[1];

        return retPosition;
    }

    /**
     * @param position the position to set
     */
    public UpdateStatus setFigurePosition(Integer[] position) {
        if (Arrays.equals(dimensions.position, position)) {
            return UpdateStatus.NoChange;
        }

        dimensions.position[0] = position[0];
        dimensions.position[1] = position[1];
        return UpdateStatus.Success;
    }

    public UpdateStatus setPosition(Integer[] position) {
        if (Arrays.equals(dimensions.position, position)) {
            return UpdateStatus.NoChange;
        }

        dimensions.position[0] = position[0];
        dimensions.position[1] = position[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the figure size
     */
    public Integer[] getFigureSize() {
        return getSize();
    }

    public Integer[] getSize() {
        Integer[] retSize = new Integer[2];

        retSize[0] = dimensions.size[0];
        retSize[1] = dimensions.size[1];

        return retSize;
    }

    /**
     * @param size the size to set
     */
    public UpdateStatus setFigureSize(Integer[] size) {
        return setSize(size);
    }

    public UpdateStatus setSize(Integer[] size) {
        if (Arrays.equals(dimensions.size, size)) {
            return UpdateStatus.NoChange;
        }

        dimensions.size[0] = size[0];
        dimensions.size[1] = size[1];
        return UpdateStatus.Success;
    }

    /**
     * @return the eventHandler
     */
    public EventHandler getEventHandlerObject() {
        return eventHandler;
    }

    /**
     * @param eventHandler the eventHandler to set
     */
    public UpdateStatus setEventHandlerObject(EventHandler eventHandler) {
        if (this.eventHandler.equals(eventHandler)) {
            return UpdateStatus.NoChange;
        }
        this.eventHandler = eventHandler;
        return UpdateStatus.Success;
    }

    /**
     * @return the eventHandler string
     */
    public String getEventHandler() {
        return eventHandler.eventHandler;
    }

    /**
     * @param eventHandlerString the eventHandler string to set
     */
    public UpdateStatus setEventHandler(String eventHandlerString) {
        if (eventHandler.eventHandler.equals(eventHandlerString)) {
            return UpdateStatus.NoChange;
        }
        eventHandler.eventHandler = eventHandlerString;
        return UpdateStatus.Success;
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
    public UpdateStatus setEventHandlerEnable(boolean eventHandlerEnabled) {
        if (eventHandler.eventHandlerEnabled == eventHandlerEnabled) {
            return UpdateStatus.NoChange;
        }
        eventHandler.eventHandlerEnabled = eventHandlerEnabled;
        return UpdateStatus.Success;
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
    public UpdateStatus setInfoMessage(String infoMessage) {
        if (this.infoMessage != null && this.infoMessage.equals(infoMessage)) {
            return UpdateStatus.NoChange;
        }
        this.infoMessage = infoMessage;
        return UpdateStatus.Success;
    }

    /**
     * @return the name
     */
    public String getFigureName() {
        return figureName.name;
    }

    /**
     * @param figureName the figure name to set
     */
    public UpdateStatus setFigureName(String figureName) {
        if (this.figureName.name.equals(figureName)) {
            return UpdateStatus.NoChange;
        }
        this.figureName.name = figureName;
        return UpdateStatus.Success;
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
    public UpdateStatus setName(String name) {
        if (figureName.name.equals(name)) {
            return UpdateStatus.NoChange;
        }
        figureName.name = name;
        return UpdateStatus.Success;
    }

    /**
     * @return the id
     */
    public Integer getFigureId() {
        return getId();
    }

    public Integer getId() {
        return figureName.id;
    }

    /**
     * @param id the id to set
     */
    public UpdateStatus setFigureId(int id) {
        return setId(id);
    }

    public UpdateStatus setId(int id) {
        //must return Success to update Views
        figureName.id = id;
        return UpdateStatus.Success;
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
    public UpdateStatus setRenderingMode(RenderingMode renderingMode) {
        if (this.renderingMode.equals(renderingMode)) {
            return UpdateStatus.NoChange;
        }
        this.renderingMode = renderingMode;
        return UpdateStatus.Success;
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
    public UpdateStatus setPixmap(boolean pixmap) {
        if (renderingMode.pixmap == pixmap) {
            return UpdateStatus.NoChange;
        }
        renderingMode.pixmap = pixmap;
        return UpdateStatus.Success;
    }

    /**
     * @return the pixel drawing mode enum
     */
    public PixelDrawingMode getPixelDrawingMode() {
        return renderingMode.pixelDrawingMode;
    }

    /**
     * @return the pixel drawing mode
     */
    public Integer getPixelDrawingModeAsInteger() {
        return getPixelDrawingMode().ordinal();
    }

    /**
     * @param pixelDrawingMode the pixel drawing mode to set
     */
    public UpdateStatus setPixelDrawingMode(PixelDrawingMode pixelDrawingMode) {
        if (renderingMode.pixelDrawingMode.equals(pixelDrawingMode)) {
            return UpdateStatus.NoChange;
        }
        renderingMode.pixelDrawingMode = pixelDrawingMode;
        return UpdateStatus.Success;
    }

    /**
     * @param pixelDrawingMode the pixel drawing mode to set
     */
    public UpdateStatus setPixelDrawingMode(Integer pixelDrawingMode) {
        PixelDrawingMode mode = PixelDrawingMode.intToEnum(pixelDrawingMode);
        if (renderingMode.pixelDrawingMode == mode) {
            return UpdateStatus.NoChange;
        }

        renderingMode.pixelDrawingMode = mode;
        return UpdateStatus.Success;
    }

    /**
     * @return the pixmap
     */
    public Antialiasing getAntialiasing() {
        return renderingMode.antialiasing;
    }

    /**
     * @param antialiasing the antialiasing to set
     */
    public UpdateStatus setAntialiasing(Antialiasing antialiasing) {
        if (renderingMode.antialiasing == antialiasing) {
            return UpdateStatus.NoChange;
        }
        renderingMode.antialiasing = antialiasing;
        return UpdateStatus.Success;
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
    public UpdateStatus setImmediateDrawing(boolean immediateDrawing) {
        if (renderingMode.immediateDrawing == immediateDrawing) {
            return UpdateStatus.NoChange;
        }

        renderingMode.immediateDrawing = immediateDrawing;
        return UpdateStatus.Success;
    }

    /**
     * @return the rotation
     */
    public RotationType getRotationStyle() {
        return rotation;
    }

    /**
     * @return the rotation
     */
    public Integer getRotationStyleAsInteger() {
        return getRotationStyle().ordinal();
    }

    /**
     * @param rotation the rotation to set
     */
    public UpdateStatus setRotationStyle(RotationType rotation) {
        if (this.rotation.equals(rotation)) {
            return UpdateStatus.NoChange;
        }
        this.rotation = rotation;
        return UpdateStatus.Success;
    }

    /**
     * @param rotation the rotation to set
     */
    public UpdateStatus setRotationStyle(Integer rotation) {
        RotationType rotationType = RotationType.intToEnum(rotation);
        if (rotationType == null || this.rotation == rotationType) {
            return UpdateStatus.NoChange;
        }

        this.rotation = rotationType;
        return UpdateStatus.Success;
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
    public UpdateStatus setResizeFcn(String resizeFcn) {
        if (this.resizeFcn.equals(resizeFcn)) {
            return UpdateStatus.NoChange;
        }
        this.resizeFcn = resizeFcn;
        return UpdateStatus.Success;
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
    public UpdateStatus setCloseRequestFcn(String closeRequestFcn) {
        if (this.closeRequestFcn.equals(closeRequestFcn)) {
            return UpdateStatus.NoChange;
        }
        this.closeRequestFcn = closeRequestFcn;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_FIGURE__;
    }

}
