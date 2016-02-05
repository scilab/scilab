/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.figure;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ANTIALIASING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BACKGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSEREQUESTFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLORMAP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLORMAP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DEFAULT_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DOCKABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT_SET__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZEFCN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ROTATION_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VIEWPORT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;

import java.util.Arrays;

import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.event.EventHandler;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.LayoutType;

/**
 * Figure class
 * @author Manuel JULIACHS
 */
public class Figure extends GraphicObject implements AxesContainer {
    /** Figure properties names */
    private enum FigureProperty {
        INFOMESSAGE, COLORMAP, COLORMAPSIZE, BACKGROUND, ROTATIONTYPE, RESIZEFCN, CLOSEREQUESTFCN, RESIZE, TOOLBAR, TOOLBAR_VISIBLE, MENUBAR, MENUBAR_VISIBLE, INFOBAR_VISIBLE, DOCKABLE, LAYOUT, LAYOUT_SET, GRIDOPT_GRID, GRIDOPT_PADDING, BORDEROPT_PADDING, DEFAULT_AXES, ICON
    };

    /**
     * Specifies whether rotation applies to a single subwindow or to all the
     * figure's subwindows
     */
    public enum RotationType {
        UNARY, MULTIPLE;

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

        public static RotationType stringToEnum(String value) {
            if (value.equals("multiple")) {
                return MULTIPLE;
            }

            return UNARY;
        }

        public static String enumToString(RotationType value) {
            switch (value) {
                case MULTIPLE:
                    return "multiple";
                case UNARY:
                default:
                    return "unary";
            }
        }
    }

    public enum BarType {
        NONE, FIGURE;
        public static BarType intToEnum(Integer intValue) {
            switch (intValue) {
                default:
                case 0:
                    return BarType.NONE;
                case 1:
                    return BarType.FIGURE;
            }
        }

        public static BarType stringToEnum(String value) {
            if (value.equals("figure")) {
                return FIGURE;
            }

            return NONE;
        }

        public static String enumToString(BarType value) {
            switch (value) {
                case FIGURE:
                    return "figure";
                case NONE:
                default:
                    return "none";
            }
        }
    }

    /** Pixel drawing logical operations */
    private enum PixelDrawingMode {
        CLEAR, AND, ANDREVERSE, COPY, ANDINVERTED, NOOP, XOR, OR, NOR, EQUIV, INVERT, ORREVERSE, COPYINVERTED, ORINVERTED, NAND, SET;

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
    public enum FigureDimensionsProperty {
        POSITION, SIZE
    };

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
            position = new Integer[] { 0, 0 };
            size = new Integer[] { 0, 0 };
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
    public enum CanvasProperty {
        AUTORESIZE, VIEWPORT, AXESSIZE
    };

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
            viewport = new Integer[2];
            axesSize = new Integer[2];
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
    public enum FigureNameProperty {
        NAME, ID
    };

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
    public enum RenderingModeProperty {
        PIXELDRAWINGMODE, ANTIALIASING, IMMEDIATEDRAWING
    };

    /**
     * RenderingMode class
     */
    private class RenderingMode {

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
            pixelDrawingMode = PixelDrawingMode.COPY;
            antialiasing = 0;
            immediateDrawing = true;
        }

        /**
         * Copy constructor
         * @param renderingMode the RenderingMode to copy
         */
        public RenderingMode(RenderingMode renderingMode) {
            pixelDrawingMode = renderingMode.pixelDrawingMode;
            antialiasing = renderingMode.antialiasing;
            immediateDrawing = renderingMode.immediateDrawing;
        }

    }

    /** EventHandler properties names */
    public enum EventHandlerProperty {
        EVENTHANDLER, EVENTHANDLERENABLE
    };

    /** Figure dimensions */
    private FigureDimensions dimensions;

    /** Canvas property */
    private Canvas canvas;

    /** Figure name */
    private FigureName figureName;

    /** Info message */
    private String infoMessage;

    /**
     * Default ColorMap: (3 x N) matrix, where N is the number of colors and 3
     * the number of color channels
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

    /** resize */
    private Boolean resize;

    /** toolbar */
    private BarType toolbar;
    private Boolean toolbarVisible;

    /** menubar */
    private BarType menubar;
    private Boolean menubarVisible;

    /** infobar */
    private Boolean infobarVisible;

    /** dockable */
    private Boolean dockable;

    /** layout */
    private LayoutType layout;

    /** layout options */
    private Integer[] gridOptGrid = new Integer[] { 0, 0 };
    private Integer[] gridOptPadding = new Integer[] { 0, 0 };
    private Integer[] borderOptPadding = new Integer[] { 0, 0 };

    /** default axes management */
    private Boolean defaultAxes;

    private String icon = "";

    /** Constructor */
    public Figure() {
        super();
        dimensions = new FigureDimensions();
        canvas = new Canvas();
        figureName = new FigureName();
        infoMessage = "";
        colorMap = new ColorMap();

        renderingMode = new RenderingMode();
        background = 0;
        eventHandler = new EventHandler();
        resizeFcn = "";
        closeRequestFcn = "";
        rotation = RotationType.UNARY;
        resize = true;
        toolbarVisible = true;
        toolbar = BarType.FIGURE;
        menubarVisible = true;
        menubar = BarType.FIGURE;
        infobarVisible = true;
        dockable = true;
        layout = LayoutType.NONE;
        defaultAxes = true;
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

        copy.gridOptGrid = new Integer[] { 0, 0 };
        copy.gridOptPadding = new Integer[] { 0, 0 };
        copy.borderOptPadding = new Integer[] { 0, 0 };
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
            case __GO_POSITION__:
                return FigureDimensionsProperty.POSITION;
            case __GO_SIZE__:
                return FigureDimensionsProperty.SIZE;
            case __GO_AUTORESIZE__:
                return CanvasProperty.AUTORESIZE;
            case __GO_VIEWPORT__:
                return CanvasProperty.VIEWPORT;
            case __GO_AXES_SIZE__:
                return CanvasProperty.AXESSIZE;
            case __GO_NAME__:
                return FigureNameProperty.NAME;
            case __GO_ID__:
                return FigureNameProperty.ID;
            case __GO_INFO_MESSAGE__:
                return FigureProperty.INFOMESSAGE;
            case __GO_COLORMAP__:
                return FigureProperty.COLORMAP;
            case __GO_COLORMAP_SIZE__:
                return FigureProperty.COLORMAPSIZE;
            case __GO_PIXEL_DRAWING_MODE__:
                return RenderingModeProperty.PIXELDRAWINGMODE;
            case __GO_ANTIALIASING__:
                return RenderingModeProperty.ANTIALIASING;
            case __GO_IMMEDIATE_DRAWING__:
                return RenderingModeProperty.IMMEDIATEDRAWING;
            case __GO_BACKGROUND__:
                return FigureProperty.BACKGROUND;
            case __GO_EVENTHANDLER_NAME__:
                return EventHandlerProperty.EVENTHANDLER;
            case __GO_EVENTHANDLER_ENABLE__:
                return EventHandlerProperty.EVENTHANDLERENABLE;
            case __GO_ROTATION_TYPE__:
                return FigureProperty.ROTATIONTYPE;
            case __GO_RESIZEFCN__:
                return FigureProperty.RESIZEFCN;
            case __GO_CLOSEREQUESTFCN__:
                return FigureProperty.CLOSEREQUESTFCN;
            case __GO_RESIZE__:
                return FigureProperty.RESIZE;
            case __GO_TOOLBAR__:
                return FigureProperty.TOOLBAR;
            case __GO_TOOLBAR_VISIBLE__:
                return FigureProperty.TOOLBAR_VISIBLE;
            case __GO_MENUBAR__:
                return FigureProperty.MENUBAR;
            case __GO_MENUBAR_VISIBLE__:
                return FigureProperty.MENUBAR_VISIBLE;
            case __GO_INFOBAR_VISIBLE__:
                return FigureProperty.INFOBAR_VISIBLE;
            case __GO_DOCKABLE__:
                return FigureProperty.DOCKABLE;
            case __GO_LAYOUT__:
                return FigureProperty.LAYOUT;
            case __GO_LAYOUT_SET__:
                return FigureProperty.LAYOUT_SET;
            case __GO_GRID_OPT_GRID__:
                return FigureProperty.GRIDOPT_GRID;
            case __GO_GRID_OPT_PADDING__:
                return FigureProperty.GRIDOPT_PADDING;
            case __GO_BORDER_OPT_PADDING__:
                return FigureProperty.BORDEROPT_PADDING;
            case __GO_DEFAULT_AXES__:
                return FigureProperty.DEFAULT_AXES;
            case __GO_UI_ICON__:
                return FigureProperty.ICON;
            default:
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
        } else if (property == FigureProperty.ROTATIONTYPE) {
            return getRotation();
        } else if (property == FigureProperty.RESIZEFCN) {
            return getResizeFcn();
        } else if (property == FigureProperty.CLOSEREQUESTFCN) {
            return getCloseRequestFcn();
        } else if (property == FigureProperty.RESIZE) {
            return getResize();
        } else if (property == FigureProperty.TOOLBAR) {
            return getToolbar();
        } else if (property == FigureProperty.TOOLBAR_VISIBLE) {
            return getToolbarVisible();
        } else if (property == FigureProperty.MENUBAR) {
            return getMenubar();
        } else if (property == FigureProperty.MENUBAR_VISIBLE) {
            return getMenubarVisible();
        } else if (property == FigureProperty.INFOBAR_VISIBLE) {
            return getInfobarVisible();
        } else if (property == FigureProperty.DOCKABLE) {
            return getDockable();
        } else if (property == FigureProperty.LAYOUT) {
            return getLayout();
        } else if (property == FigureProperty.LAYOUT_SET) {
            return isLayoutSettable();
        } else if (property == FigureProperty.GRIDOPT_GRID) {
            return getGridOptGrid();
        } else if (property == FigureProperty.GRIDOPT_PADDING) {
            return getGridOptPadding();
        } else if (property == FigureProperty.BORDEROPT_PADDING) {
            return getBorderOptPadding();
        } else if (property == FigureProperty.DEFAULT_AXES) {
            return hasDefaultAxes();
        } else if (property == FigureProperty.ICON) {
            return getIcon();
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
            switch ((FigureProperty) property) {
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
                    return setRotation((Integer) value);
                case RESIZE:
                    return setResize((Boolean) value);
                case TOOLBAR:
                    return setToolbar((Integer) value);
                case TOOLBAR_VISIBLE:
                    return setToolbarVisible((Boolean) value);
                case MENUBAR:
                    return setMenubar((Integer) value);
                case MENUBAR_VISIBLE:
                    return setMenubarVisible((Boolean) value);
                case INFOBAR_VISIBLE:
                    return setInfobarVisible((Boolean) value);
                case DOCKABLE:
                    return setDockable((Boolean) value);
                case LAYOUT:
                    return setLayout((Integer) value);
                case GRIDOPT_GRID:
                    return setGridOptGrid((Integer[]) value);
                case GRIDOPT_PADDING:
                    return setGridOptPadding((Integer[]) value);
                case BORDEROPT_PADDING:
                    return setBorderOptPadding((Integer[]) value);
                case DEFAULT_AXES:
                    return setDefaultAxes((Boolean) value);
                case ICON:
                    return setIcon((String) value);
                default:
                    break;
            }
        } else if (property instanceof CanvasProperty) {
            switch ((CanvasProperty) property) {
                case AUTORESIZE:
                    return setAutoResize((Boolean) value);
                case AXESSIZE:
                    return setAxesSize((Integer[]) value);
                case VIEWPORT:
                    return setViewport((Integer[]) value);
            }
        } else if (property instanceof FigureDimensionsProperty) {
            switch ((FigureDimensionsProperty) property) {
                case POSITION:
                    return setPosition((Integer[]) value);
                case SIZE:
                    return setSize((Integer[]) value);
            }
        } else if (property instanceof FigureNameProperty) {
            switch ((FigureNameProperty) property) {
                case ID:
                    return setId((Integer) value);
                case NAME:
                    return setName((String) value);
            }
        } else if (property instanceof RenderingModeProperty) {
            switch ((RenderingModeProperty) property) {
                case ANTIALIASING:
                    return setAntialiasing((Integer) value);
                case IMMEDIATEDRAWING:
                    return setImmediateDrawing((Boolean) value);
                case PIXELDRAWINGMODE:
                    return setPixelDrawingMode((Integer) value);
            }
        } else if (property instanceof EventHandlerProperty) {
            switch ((EventHandlerProperty) property) {
                case EVENTHANDLER:
                    return setEventHandlerString((String) value);
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
    public UpdateStatus setBackground(Integer background) {
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
    public UpdateStatus setAutoResize(Boolean autoResize) {
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
    public Integer[] getPosition() {
        Integer[] retPosition = new Integer[2];

        retPosition[0] = dimensions.position[0];
        retPosition[1] = dimensions.position[1];

        return retPosition;
    }

    /**
     * @param position the position to set
     */
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
    public Integer[] getSize() {
        Integer[] retSize = new Integer[2];

        retSize[0] = dimensions.size[0];
        retSize[1] = dimensions.size[1];

        return retSize;
    }

    /**
     * @param size the size to set
     */
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
    public EventHandler getEventHandler() {
        return eventHandler;
    }

    /**
     * @param eventHandler the eventHandler to set
     */
    public UpdateStatus setEventHandler(EventHandler eventHandler) {
        if (this.eventHandler.equals(eventHandler)) {
            return UpdateStatus.NoChange;
        }
        this.eventHandler = eventHandler;
        return UpdateStatus.Success;
    }

    /**
     * @return the eventHandler string
     */
    public String getEventHandlerString() {
        return eventHandler.getEventHandlerString();
    }

    /**
     * @param eventHandlerString the eventHandler string to set
     */
    public UpdateStatus setEventHandlerString(String eventHandlerString) {
        if (eventHandler.getEventHandlerString().equals(eventHandlerString)) {
            return UpdateStatus.NoChange;
        }
        eventHandler.setEventHandlerString(eventHandlerString);
        return UpdateStatus.Success;
    }

    /**
     * @return the eventHandlerEnabled
     */
    public Boolean getEventHandlerEnable() {
        return eventHandler.getEventHandlerEnabled();
    }

    /**
     * @param eventHandlerEnabled the eventHandlerEnabled to set
     */
    public UpdateStatus setEventHandlerEnable(Boolean eventHandlerEnabled) {
        if (eventHandler.getEventHandlerEnabled() == eventHandlerEnabled) {
            return UpdateStatus.NoChange;
        }
        eventHandler.setEventHandlerEnabled(eventHandlerEnabled);
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
    public FigureName getFigureName() {
        return figureName;
    }

    /**
     * @param figureName the figure name to set
     */
    public UpdateStatus setFigureName(FigureName figureName) {
        if (this.figureName.equals(figureName)) {
            return UpdateStatus.NoChange;
        }
        this.figureName = figureName;
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
    public Integer getId() {
        return figureName.id;
    }

    /**
     * @param id the id to set
     */
    public UpdateStatus setId(Integer id) {
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
     * @return the antialiasing
     */
    public Integer getAntialiasing() {
        return renderingMode.antialiasing;
    }

    /**
     * @param antialiasing the antialiasing to set
     */
    public UpdateStatus setAntialiasing(Integer antialiasing) {
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
    public UpdateStatus setImmediateDrawing(Boolean immediateDrawing) {
        if (renderingMode.immediateDrawing == immediateDrawing) {
            return UpdateStatus.NoChange;
        }

        renderingMode.immediateDrawing = immediateDrawing;
        return UpdateStatus.Success;
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
    public UpdateStatus setRotation(RotationType rotation) {
        if (this.rotation.equals(rotation)) {
            return UpdateStatus.NoChange;
        }
        this.rotation = rotation;
        return UpdateStatus.Success;
    }

    /**
     * @param rotation the rotation to set
     */
    public UpdateStatus setRotation(Integer rotation) {
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

    public Boolean getResize() {
        return resize;
    }

    public UpdateStatus setResize(Boolean status) {
        if (status.equals(resize)) {
            return UpdateStatus.NoChange;
        }

        resize = status;
        return UpdateStatus.Success;
    }

    public Boolean getToolbarVisible() {
        return toolbarVisible;
    }

    public UpdateStatus setToolbarVisible(Boolean status) {
        if (status.equals(toolbarVisible)) {
            return UpdateStatus.NoChange;
        }

        toolbarVisible = status;
        return UpdateStatus.Success;
    }

    public Integer getToolbar() {
        return toolbar.ordinal();
    }

    public BarType getToolbarAsEnum() {
        return toolbar;
    }

    public UpdateStatus setToolbar(Integer toolbar) {
        return setToolbar(BarType.intToEnum(toolbar));
    }

    public UpdateStatus setToolbar(BarType toolbar) {
        if (toolbar == this.toolbar) {
            return UpdateStatus.NoChange;
        }

        this.toolbar = toolbar;
        return UpdateStatus.Success;
    }

    public Boolean getMenubarVisible() {
        return menubarVisible;
    }

    public UpdateStatus setMenubarVisible(Boolean status) {
        if (status.equals(menubarVisible)) {
            return UpdateStatus.NoChange;
        }

        menubarVisible = status;
        return UpdateStatus.Success;
    }

    public Integer getMenubar() {
        return menubar.ordinal();
    }

    public BarType getMenubarAsEnum() {
        return menubar;
    }

    public UpdateStatus setMenubar(Integer menubar) {
        return setMenubar(BarType.intToEnum(menubar));
    }

    public UpdateStatus setMenubar(BarType menubar) {
        if (menubar == this.menubar) {
            return UpdateStatus.NoChange;
        }

        this.menubar = menubar;
        return UpdateStatus.Success;
    }

    public Boolean getInfobarVisible() {
        return infobarVisible;
    }

    public UpdateStatus setInfobarVisible(Boolean status) {
        if (status.equals(infobarVisible)) {
            return UpdateStatus.NoChange;
        }

        infobarVisible = status;
        return UpdateStatus.Success;
    }

    public Boolean getDockable() {
        return dockable;
    }

    public UpdateStatus setDockable(Boolean status) {
        if (status.equals(dockable)) {
            return UpdateStatus.NoChange;
        }

        dockable = status;
        return UpdateStatus.Success;
    }

    public Integer getLayout() {
        return layout.ordinal();
    }

    public LayoutType getLayoutAsEnum() {
        return layout;
    }

    public boolean isLayoutSettable() {
        return (this.layout == LayoutType.NONE);
    }

    public UpdateStatus setLayout(Integer value) {
        return setLayout(LayoutType.intToEnum(value));
    }

    public UpdateStatus setLayout(LayoutType layout) {
        //avoid to set layout twice
        if (this.layout == LayoutType.NONE) {
            if (layout == LayoutType.NONE) {
                return UpdateStatus.NoChange;
            }

            this.layout = layout;
            return UpdateStatus.Success;
        }

        return UpdateStatus.Fail;
    }

    public Integer[] getBorderOptPadding() {
        return borderOptPadding;
    }

    public UpdateStatus setBorderOptPadding(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (borderOptPadding.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0; i < value.length; i++) {
            if (borderOptPadding[i] != value[i]) {
                borderOptPadding[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridOptPadding() {
        return gridOptPadding;
    }

    public UpdateStatus setGridOptPadding(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridOptPadding.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0; i < value.length; i++) {
            if (gridOptPadding[i] != value[i]) {
                gridOptPadding[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridOptGrid() {
        return gridOptGrid;
    }

    public UpdateStatus setGridOptGrid(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridOptGrid.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0; i < value.length; i++) {
            if (gridOptGrid[i] != value[i]) {
                gridOptGrid[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Boolean hasDefaultAxes() {
        return defaultAxes;
    }

    public UpdateStatus setDefaultAxes(Boolean status) {
        if (status.equals(defaultAxes)) {
            return UpdateStatus.NoChange;
        }

        defaultAxes = status;
        return UpdateStatus.Success;
    }

    public String getIcon() {
        return icon;
    }

    public UpdateStatus setIcon(String icon) {
        if (this.icon.equals(icon)) {
            return UpdateStatus.NoChange;
        }

        this.icon = icon;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_FIGURE__;
    }

}
