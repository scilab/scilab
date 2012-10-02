/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axes;

import org.scilab.modules.graphic_objects.arc.Arc.ArcDrawingMethod;
import org.scilab.modules.graphic_objects.arc.Arc.ArcProperty;
import org.scilab.modules.graphic_objects.axes.AxisProperty.AxisLocation;
import org.scilab.modules.graphic_objects.axes.Box.BoxType;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.contouredObject.Line.LinePropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Line.LineType;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkPropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClippablePropertyType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.textObject.FormattedText;

import java.util.ArrayList;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Axes class
 * @author Manuel JULIACHS
 */
public class Axes extends GraphicObject {

    private static final double BOUNDS_PARAMETER = 5;

    /** Axes properties names */
    private enum AxesProperty {
        XAXISVISIBLE, XAXISREVERSE, XAXISGRIDCOLOR, XAXISLABEL, XAXISLOCATION, XAXISLOGFLAG,
        XAXISTICKS, XAXISAUTOTICKS, XAXISNUMBERTICKS, XAXISTICKSLOCATIONS, XAXISTICKSLABELS, XAXISSUBTICKS,
        YAXISVISIBLE, YAXISREVERSE, YAXISGRIDCOLOR, YAXISLABEL, YAXISLOCATION, YAXISLOGFLAG,
        YAXISTICKS, YAXISAUTOTICKS, YAXISNUMBERTICKS, YAXISTICKSLOCATIONS, YAXISTICKSLABELS, YAXISSUBTICKS,
        ZAXISVISIBLE, ZAXISREVERSE, ZAXISGRIDCOLOR, ZAXISLABEL, ZAXISLOCATION, ZAXISLOGFLAG,
        ZAXISTICKS, ZAXISAUTOTICKS, ZAXISNUMBERTICKS, ZAXISTICKSLOCATIONS, ZAXISTICKSLABELS, ZAXISSUBTICKS,
        AUTOSUBTICKS,
        FONT_STYLE, FONT_SIZE, FONT_COLOR, FONT_FRACTIONAL,
        GRIDPOSITION, TITLE, AUTOCLEAR, FILLED, BACKGROUND,
        MARGINS, AXESBOUNDS,
        HIDDENCOLOR
    };

    /** Specifies the grid position relative to the graphics entities */
    public static enum GridPosition { BACKGROUND, FOREGROUND;

                                      /**
                                       * Converts an integer to the corresponding enum
                                       * @param intValue the integer value
                                       * @return the grid position enum
                                       */
    public static GridPosition intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return GridPosition.BACKGROUND;
            case 1:
                return GridPosition.FOREGROUND;
            default:
                return null;
        }
    }
                                    };


    /** 3-element array (properties of the X, Y and Z axes) */
    private AxisProperty[] axes;

    /**
     * Specifies whether subticks are automatically computed or not
     * Used as an internal state only
     * Note: shared by the three axes to be compatible with the
     * former flagNax internal state (autoSubticks is equivalent to !flagNax)
     * This should eventually become a per-axis property (as the auto ticks flag).
     */
    private boolean autoSubticks;

    /** Grid position */
    private GridPosition gridPosition;

    /** Title label known by its UID. */
    private String title;

    /** Specifies whether the Axes subwindow is cleared when a new plot command is performed */
    private boolean autoClear;

    /** Specifies whether the Axes background is filled or not  */
    private boolean filled;

    /** Axes background color and child objects default background color */
    private int background;

    /** Camera */
    private Camera camera;

    /** Box: properties related to the data bounds */
    private Box box;

    /** Margins enclosing the drawing area (left, right, top, bottom) */
    private double[] margins;

    /**
     * Axes bounds relative to their parent figure's
     * drawing area (upper-left corner x and y, width, height)
     */
    private double[] axesBounds;

    /** Default hidden surfaces color */
    private int hiddenColor;

    /** Default Line properties */
    private Line line;

    /** Default Mark properties */
    private Mark mark;

    /** Default Arc drawing method */
    private ArcDrawingMethod arcDrawingMethod;

    /** Default ClippableProperty */
    private ClippableProperty clipProperty;


    /** Constructor */
    public Axes() {
        super();
        axes = new AxisProperty[3];
        axes[0] = new AxisProperty();
        axes[1] = new AxisProperty();
        axes[2] = new AxisProperty();
        gridPosition = GridPosition.FOREGROUND;
        title = "";
        autoClear = false;
        filled = false;
        camera = new Camera();
        box = new Box();
        margins = new double[4];
        axesBounds = new double[4];

        line = new Line();
        mark = new Mark();
        arcDrawingMethod = ArcDrawingMethod.LINES;
        clipProperty = new ClippableProperty();
    }

    public Axes clone() {
        Axes copy = (Axes) super.clone();

        AxisProperty [] newAxes = new AxisProperty[3];

        for (int i = 0; i < axes.length; i++) {
            newAxes[i] = new AxisProperty(axes[i]);
        }

        copy.axes = newAxes;

        copy.title = "";

        copy.camera = new Camera(this.camera);
        copy.box = new Box(this.box);

        double [] newMargins = new double[4];

        for (int i = 0; i < this.margins.length; i++) {
            newMargins[i] = this.margins[i];
        }

        copy.margins = newMargins;

        double [] newAxesBounds = new double[4];

        for (int i = 0; i < this.axesBounds.length; i++) {
            newAxesBounds[i] = this.axesBounds[i];
        }

        copy.axesBounds = newAxesBounds;

        copy.line = new Line(this.line);
        copy.mark = new Mark(this.mark);

        copy.clipProperty = new ClippableProperty(this.clipProperty);

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
        case __GO_X_AXIS_VISIBLE__ :
            return AxesProperty.XAXISVISIBLE;
        case __GO_X_AXIS_REVERSE__ :
            return AxesProperty.XAXISREVERSE;
        case __GO_X_AXIS_GRID_COLOR__ :
            return AxesProperty.XAXISGRIDCOLOR;
        case __GO_X_AXIS_LABEL__ :
            return AxesProperty.XAXISLABEL;
        case __GO_X_AXIS_LOCATION__ :
            return AxesProperty.XAXISLOCATION;
        case __GO_X_AXIS_LOG_FLAG__ :
            return AxesProperty.XAXISLOGFLAG;
        case __GO_X_AXIS_TICKS__ :
            return AxesProperty.XAXISTICKS;
        case __GO_X_AXIS_AUTO_TICKS__ :
            return AxesProperty.XAXISAUTOTICKS;
        case __GO_X_AXIS_NUMBER_TICKS__ :
            return AxesProperty.XAXISNUMBERTICKS;
        case __GO_X_AXIS_TICKS_LOCATIONS__:
            return AxesProperty.XAXISTICKSLOCATIONS;
        case __GO_X_AXIS_TICKS_LABELS__ :
            return AxesProperty.XAXISTICKSLABELS;
        case __GO_X_AXIS_SUBTICKS__ :
            return AxesProperty.XAXISSUBTICKS;
        case __GO_Y_AXIS_VISIBLE__ :
            return AxesProperty.YAXISVISIBLE;
        case __GO_Y_AXIS_REVERSE__ :
            return AxesProperty.YAXISREVERSE;
        case __GO_Y_AXIS_GRID_COLOR__ :
            return AxesProperty.YAXISGRIDCOLOR;
        case __GO_Y_AXIS_LABEL__ :
            return AxesProperty.YAXISLABEL;
        case __GO_Y_AXIS_LOCATION__ :
            return AxesProperty.YAXISLOCATION;
        case __GO_Y_AXIS_LOG_FLAG__ :
            return AxesProperty.YAXISLOGFLAG;
        case __GO_Y_AXIS_TICKS__ :
            return AxesProperty.YAXISTICKS;
        case __GO_Y_AXIS_AUTO_TICKS__ :
            return AxesProperty.YAXISAUTOTICKS;
        case __GO_Y_AXIS_NUMBER_TICKS__ :
            return AxesProperty.YAXISNUMBERTICKS;
        case __GO_Y_AXIS_TICKS_LOCATIONS__ :
            return AxesProperty.YAXISTICKSLOCATIONS;
        case __GO_Y_AXIS_TICKS_LABELS__ :
            return AxesProperty.YAXISTICKSLABELS;
        case __GO_Y_AXIS_SUBTICKS__ :
            return AxesProperty.YAXISSUBTICKS;
        case __GO_Z_AXIS_VISIBLE__ :
            return AxesProperty.ZAXISVISIBLE;
        case __GO_Z_AXIS_REVERSE__ :
            return AxesProperty.ZAXISREVERSE;
        case __GO_Z_AXIS_GRID_COLOR__ :
            return AxesProperty.ZAXISGRIDCOLOR;
        case __GO_Z_AXIS_LABEL__ :
            return AxesProperty.ZAXISLABEL;
        case __GO_Z_AXIS_LOCATION__ :
            return AxesProperty.ZAXISLOCATION;
        case __GO_Z_AXIS_LOG_FLAG__ :
            return AxesProperty.ZAXISLOGFLAG;
        case __GO_Z_AXIS_TICKS__ :
            return AxesProperty.ZAXISTICKS;
        case __GO_Z_AXIS_AUTO_TICKS__ :
            return AxesProperty.ZAXISAUTOTICKS;
        case __GO_Z_AXIS_NUMBER_TICKS__ :
            return AxesProperty.ZAXISNUMBERTICKS;
        case __GO_Z_AXIS_TICKS_LOCATIONS__ :
            return AxesProperty.ZAXISTICKSLOCATIONS;
        case __GO_Z_AXIS_TICKS_LABELS__ :
            return AxesProperty.ZAXISTICKSLABELS;
        case __GO_Z_AXIS_SUBTICKS__ :
            return AxesProperty.ZAXISSUBTICKS;
        case __GO_AUTO_SUBTICKS__ :
            return AxesProperty.AUTOSUBTICKS;
        case __GO_FONT_STYLE__ :
            return AxesProperty.FONT_STYLE;
        case __GO_FONT_SIZE__ :
            return AxesProperty.FONT_SIZE;
        case __GO_FONT_COLOR__ :
            return AxesProperty.FONT_COLOR;
        case __GO_FONT_FRACTIONAL__ :
            return AxesProperty.FONT_FRACTIONAL;
        case __GO_GRID_POSITION__ :
            return AxesProperty.GRIDPOSITION;
        case __GO_TITLE__ :
            return AxesProperty.TITLE;
        case __GO_AUTO_CLEAR__ :
            return AxesProperty.AUTOCLEAR;
        case __GO_FILLED__ :
            return AxesProperty.FILLED;
        case __GO_BACKGROUND__ :
            return AxesProperty.BACKGROUND;
        case __GO_VIEW__ :
            return Camera.CameraProperty.VIEW;
        case __GO_ISOVIEW__ :
            return Camera.CameraProperty.ISOVIEW;
        case __GO_CUBE_SCALING__ :
            return Camera.CameraProperty.CUBESCALING;
        case __GO_ROTATION_ANGLES__ :
            return Camera.CameraProperty.ROTATIONANGLES;
        case __GO_ROTATION_ANGLES_3D__ :
            return Camera.CameraProperty.ROTATIONANGLES3D;
        case __GO_BOX_TYPE__ :
            return Box.BoxProperty.BOX;
        case __GO_HIDDEN_AXIS_COLOR__ :
            return Box.BoxProperty.HIDDENAXISCOLOR;
        case __GO_TIGHT_LIMITS__ :
            return Box.BoxProperty.TIGHTLIMITS;
        case __GO_DATA_BOUNDS__ :
            return Box.BoxProperty.DATABOUNDS;
        case __GO_REAL_DATA_BOUNDS__ :
            return Box.BoxProperty.REALDATABOUNDS;
        case __GO_ZOOM_ENABLED__ :
            return Box.BoxProperty.ZOOMENABLED;
        case __GO_ZOOM_BOX__ :
            return Box.BoxProperty.ZOOMBOX;
        case __GO_AUTO_SCALE__ :
            return Box.BoxProperty.AUTOSCALE;
        case __GO_FIRST_PLOT__ :
            return Box.BoxProperty.FIRSTPLOT;
        case __GO_MARGINS__ :
            return AxesProperty.MARGINS;
        case __GO_AXES_BOUNDS__ :
            return AxesProperty.AXESBOUNDS;
        case __GO_HIDDEN_COLOR__ :
            return AxesProperty.HIDDENCOLOR;
        case __GO_LINE_MODE__ :
            return LinePropertyType.MODE;
        case __GO_LINE_STYLE__ :
            return LinePropertyType.LINESTYLE;
        case __GO_LINE_THICKNESS__ :
            return LinePropertyType.THICKNESS;
        case __GO_LINE_COLOR__ :
            return LinePropertyType.COLOR;
        case __GO_MARK_MODE__ :
            return MarkPropertyType.MODE;
        case __GO_MARK_STYLE__ :
            return MarkPropertyType.STYLE;
        case __GO_MARK_SIZE_UNIT__ :
            return MarkPropertyType.SIZEUNIT;
        case __GO_MARK_SIZE__ :
            return MarkPropertyType.SIZE;
        case __GO_MARK_FOREGROUND__ :
            return MarkPropertyType.FOREGROUND;
        case __GO_MARK_BACKGROUND__ :
            return MarkPropertyType.BACKGROUND;
        case __GO_CLIP_STATE__ :
            return ClippablePropertyType.CLIPSTATE;
        case __GO_CLIP_BOX__ :
            return ClippablePropertyType.CLIPBOX;
        case __GO_CLIP_BOX_SET__ :
            return ClippablePropertyType.CLIPBOXSET;
        case __GO_ARC_DRAWING_METHOD__ :
            return ArcProperty.ARCDRAWINGMETHOD;
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
        if (property == AxesProperty.XAXISVISIBLE) {
            return getXAxisVisible();
        } else if (property == AxesProperty.XAXISREVERSE) {
            return getXAxisReverse();
        } else if (property == AxesProperty.XAXISGRIDCOLOR) {
            return getXAxisGridColor();
        } else if (property == AxesProperty.XAXISLABEL) {
            return getXAxisLabel();
        } else if (property == AxesProperty.XAXISLOCATION) {
            return getXAxisLocation();
        } else if (property == AxesProperty.XAXISLOGFLAG) {
            return getXAxisLogFlag();
        } else if (property == AxesProperty.XAXISTICKS) {
            return getXAxisTicks();
        } else if (property == AxesProperty.XAXISAUTOTICKS) {
            return getXAxisAutoTicks();
        } else if (property == AxesProperty.XAXISNUMBERTICKS) {
            return getXAxisNumberTicks();
        } else if (property == AxesProperty.XAXISTICKSLOCATIONS) {
            return getXAxisTicksLocations();
        } else if (property == AxesProperty.XAXISTICKSLABELS) {
            return getXAxisTicksLabels();
        } else if (property == AxesProperty.XAXISSUBTICKS) {
            return getXAxisSubticks();
        } else if (property == AxesProperty.YAXISVISIBLE) {
            return getYAxisVisible();
        } else if (property == AxesProperty.YAXISREVERSE) {
            return getYAxisReverse();
        } else if (property == AxesProperty.YAXISGRIDCOLOR) {
            return getYAxisGridColor();
        } else if (property == AxesProperty.YAXISLABEL) {
            return getYAxisLabel();
        } else if (property == AxesProperty.YAXISLOCATION) {
            return getYAxisLocation();
        } else if (property == AxesProperty.YAXISLOGFLAG) {
            return getYAxisLogFlag();
        } else if (property == AxesProperty.YAXISTICKS) {
            return getYAxisTicks();
        } else if (property == AxesProperty.YAXISAUTOTICKS) {
            return getYAxisAutoTicks();
        } else if (property == AxesProperty.YAXISNUMBERTICKS) {
            return getYAxisNumberTicks();
        } else if (property == AxesProperty.YAXISTICKSLOCATIONS) {
            return getYAxisTicksLocations();
        } else if (property == AxesProperty.YAXISTICKSLABELS) {
            return getYAxisTicksLabels();
        } else if (property == AxesProperty.YAXISSUBTICKS) {
            return getYAxisSubticks();
        } else if (property == AxesProperty.ZAXISVISIBLE) {
            return getZAxisVisible();
        } else if (property == AxesProperty.ZAXISREVERSE) {
            return getZAxisReverse();
        } else if (property == AxesProperty.ZAXISGRIDCOLOR) {
            return getZAxisGridColor();
        } else if (property == AxesProperty.ZAXISLABEL) {
            return getZAxisLabel();
        } else if (property == AxesProperty.ZAXISLOCATION) {
            return getZAxisLocation();
        } else if (property == AxesProperty.ZAXISLOGFLAG) {
            return getZAxisLogFlag();
        } else if (property == AxesProperty.ZAXISTICKS) {
            return getZAxisTicks();
        } else if (property == AxesProperty.ZAXISAUTOTICKS) {
            return getZAxisAutoTicks();
        } else if (property == AxesProperty.ZAXISNUMBERTICKS) {
            return getZAxisNumberTicks();
        } else if (property == AxesProperty.ZAXISTICKSLOCATIONS) {
            return getZAxisTicksLocations();
        } else if (property == AxesProperty.ZAXISTICKSLABELS) {
            return getZAxisTicksLabels();
        } else if (property == AxesProperty.ZAXISSUBTICKS) {
            return getZAxisSubticks();
        } else if (property == AxesProperty.AUTOSUBTICKS) {
            return getAutoSubticks();
        } else if (property == AxesProperty.FONT_STYLE) {
            return getFontStyle();
        } else if (property == AxesProperty.FONT_SIZE) {
            return getFontSize();
        } else if (property == AxesProperty.FONT_COLOR) {
            return getFontColor();
        } else if (property == AxesProperty.FONT_FRACTIONAL) {
            return getFontFractional();
        } else if (property == AxesProperty.GRIDPOSITION) {
            return getGridPosition();
        } else if (property == AxesProperty.TITLE) {
            return getTitle();
        } else if (property == AxesProperty.AUTOCLEAR) {
            return getAutoClear();
        } else if (property == AxesProperty.FILLED) {
            return getFilled();
        } else if (property == AxesProperty.BACKGROUND) {
            return getBackground();
        } else if (property == Camera.CameraProperty.VIEW) {
            return getView();
        } else if (property == Camera.CameraProperty.ISOVIEW) {
            return getIsoview();
        } else if (property == Camera.CameraProperty.CUBESCALING) {
            return getCubeScaling();
        } else if (property == Camera.CameraProperty.ROTATIONANGLES) {
            return getRotationAngles();
        } else if (property == Camera.CameraProperty.ROTATIONANGLES3D) {
            return getRotationAngles3d();
        } else if (property == Box.BoxProperty.BOX) {
            return getBoxType();
        } else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
            return getHiddenAxisColor();
        } else if (property == Box.BoxProperty.TIGHTLIMITS) {
            return getTightLimits();
        } else if (property == Box.BoxProperty.DATABOUNDS) {
            return getDataBounds();
        } else if (property == Box.BoxProperty.REALDATABOUNDS) {
            return getRealDataBounds();
        } else if (property == Box.BoxProperty.ZOOMENABLED) {
            return getZoomEnabled();
        } else if (property == Box.BoxProperty.ZOOMBOX) {
            return getZoomBox();
        } else if (property == Box.BoxProperty.AUTOSCALE) {
            return getAutoScale();
        } else if (property == Box.BoxProperty.FIRSTPLOT) {
            return getFirstPlot();
        } else if (property == AxesProperty.MARGINS) {
            return getMargins();
        } else if (property == AxesProperty.AXESBOUNDS) {
            return getAxesBounds();
        } else if (property == AxesProperty.HIDDENCOLOR) {
            return getHiddenColor();
        } else if (property == LinePropertyType.MODE) {
            return getLineMode();
        } else if (property == LinePropertyType.LINESTYLE) {
            return getLineStyle();
        } else if (property == LinePropertyType.THICKNESS) {
            return getLineThickness();
        } else if (property == LinePropertyType.COLOR) {
            return getLineColor();
        } else if (property == MarkPropertyType.MODE) {
            return getMarkMode();
        } else if (property == MarkPropertyType.STYLE) {
            return getMarkStyle();
        } else if (property == MarkPropertyType.SIZE) {
            return getMarkSize();
        } else if (property == MarkPropertyType.SIZEUNIT) {
            return getMarkSizeUnit();
        } else if (property == MarkPropertyType.FOREGROUND) {
            return getMarkForeground();
        } else if (property == MarkPropertyType.BACKGROUND) {
            return getMarkBackground();
        } else if (property == ClippablePropertyType.CLIPSTATE) {
            return getClipState();
        } else if (property == ClippablePropertyType.CLIPBOX) {
            return getClipBox();
        } else if (property == ClippablePropertyType.CLIPBOXSET) {
            return getClipBoxSet();
        } else if (property == ArcProperty.ARCDRAWINGMETHOD) {
            return getArcDrawingMethod();
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
        if (property == AxesProperty.XAXISVISIBLE) {
            setXAxisVisible((Boolean) value);
        } else if (property == AxesProperty.XAXISREVERSE) {
            setXAxisReverse((Boolean) value);
        } else if (property == AxesProperty.XAXISGRIDCOLOR) {
            setXAxisGridColor((Integer) value);
        } else if (property == AxesProperty.XAXISLABEL) {
            setXAxisLabel((String) value);
        } else if (property == AxesProperty.XAXISLOCATION) {
            setXAxisLocation((Integer) value);
        } else if (property == AxesProperty.XAXISLOGFLAG) {
            setXAxisLogFlag((Boolean) value);
        } else if (property == AxesProperty.XAXISTICKS) {
            setXAxisTicks((TicksProperty) value);
        } else if (property == AxesProperty.XAXISAUTOTICKS) {
            setXAxisAutoTicks((Boolean) value);
        } else if (property == AxesProperty.XAXISTICKSLOCATIONS) {
            setXAxisTicksLocations((Double[]) value);
        } else if (property == AxesProperty.XAXISTICKSLABELS) {
            setXAxisTicksLabels((String[]) value);
        } else if (property == AxesProperty.XAXISSUBTICKS) {
            setXAxisSubticks((Integer) value);
        } else if (property == AxesProperty.YAXISVISIBLE) {
            setYAxisVisible((Boolean) value);
        } else if (property == AxesProperty.YAXISREVERSE) {
            setYAxisReverse((Boolean) value);
        } else if (property == AxesProperty.YAXISGRIDCOLOR) {
            setYAxisGridColor((Integer) value);
        } else if (property == AxesProperty.YAXISLABEL) {
            setYAxisLabel((String) value);
        } else if (property == AxesProperty.YAXISLOCATION) {
            setYAxisLocation((Integer) value);
        } else if (property == AxesProperty.YAXISLOGFLAG) {
            setYAxisLogFlag((Boolean) value);
        } else if (property == AxesProperty.YAXISTICKS) {
            setYAxisTicks((TicksProperty) value);
        } else if (property == AxesProperty.YAXISAUTOTICKS) {
            setYAxisAutoTicks((Boolean) value);
        } else if (property == AxesProperty.YAXISTICKSLOCATIONS) {
            setYAxisTicksLocations((Double[]) value);
        } else if (property == AxesProperty.YAXISTICKSLABELS) {
            setYAxisTicksLabels((String[]) value);
        } else if (property == AxesProperty.YAXISSUBTICKS) {
            setYAxisSubticks((Integer) value);
        } else if (property == AxesProperty.ZAXISVISIBLE) {
            setZAxisVisible((Boolean) value);
        } else if (property == AxesProperty.ZAXISREVERSE) {
            setZAxisReverse((Boolean) value);
        } else if (property == AxesProperty.ZAXISGRIDCOLOR) {
            setZAxisGridColor((Integer) value);
        } else if (property == AxesProperty.ZAXISLABEL) {
            setZAxisLabel((String) value);
        } else if (property == AxesProperty.ZAXISLOCATION) {
            setZAxisLocation((Integer) value);
        } else if (property == AxesProperty.ZAXISLOGFLAG) {
            setZAxisLogFlag((Boolean) value);
        } else if (property == AxesProperty.ZAXISTICKS) {
            setZAxisTicks((TicksProperty) value);
        } else if (property == AxesProperty.ZAXISAUTOTICKS) {
            setZAxisAutoTicks((Boolean) value);
        } else if (property == AxesProperty.ZAXISTICKSLOCATIONS) {
            setZAxisTicksLocations((Double[]) value);
        } else if (property == AxesProperty.ZAXISTICKSLABELS) {
            setZAxisTicksLabels((String[]) value);
        } else if (property == AxesProperty.ZAXISSUBTICKS) {
            setZAxisSubticks((Integer) value);
        } else if (property == AxesProperty.AUTOSUBTICKS) {
            setAutoSubticks((Boolean) value);
        } else if (property == AxesProperty.FONT_STYLE) {
            setFontStyle((Integer) value);
        } else if (property == AxesProperty.FONT_SIZE) {
            setFontSize((Double) value);
        } else if (property == AxesProperty.FONT_COLOR) {
            setFontColor((Integer) value);
        } else if (property == AxesProperty.FONT_FRACTIONAL) {
            setFontFractional((Boolean) value);
        } else if (property == AxesProperty.GRIDPOSITION) {
            setGridPosition((Integer) value);
        } else if (property == AxesProperty.TITLE) {
            setTitle((String) value);
        } else if (property == AxesProperty.AUTOCLEAR) {
            setAutoClear((Boolean) value);
        } else if (property == AxesProperty.FILLED) {
            setFilled((Boolean) value);
        } else if (property == AxesProperty.BACKGROUND) {
            setBackground((Integer) value);
        } else if (property == Camera.CameraProperty.VIEW) {
            setView((Integer) value);
        } else if (property == Camera.CameraProperty.ISOVIEW) {
            setIsoview((Boolean) value);
        } else if (property == Camera.CameraProperty.CUBESCALING) {
            setCubeScaling((Boolean) value);
        } else if (property == Camera.CameraProperty.ROTATIONANGLES) {
            return setRotationAngles((Double[]) value);
        } else if (property == Camera.CameraProperty.ROTATIONANGLES3D) {
            setRotationAngles3d((Double[]) value);
        } else if (property == Box.BoxProperty.BOX) {
            setBoxType((Integer) value);
        } else if (property == Box.BoxProperty.HIDDENAXISCOLOR) {
            setHiddenAxisColor((Integer) value);
        } else if (property == Box.BoxProperty.TIGHTLIMITS) {
            setTightLimits((Boolean) value);
        } else if (property == Box.BoxProperty.DATABOUNDS) {
            setDataBounds((Double[]) value);
        } else if (property == Box.BoxProperty.REALDATABOUNDS) {
            setRealDataBounds((Double[]) value);
        } else if (property == Box.BoxProperty.ZOOMENABLED) {
            setZoomEnabled((Boolean) value);
        } else if (property == Box.BoxProperty.ZOOMBOX) {
            setZoomBox((Double[]) value);
        } else if (property == Box.BoxProperty.AUTOSCALE) {
            setAutoScale((Boolean) value);
        } else if (property == Box.BoxProperty.FIRSTPLOT) {
            setFirstPlot((Boolean) value);
        } else if (property == AxesProperty.MARGINS) {
            setMargins((Double[]) value);
        } else if (property == AxesProperty.AXESBOUNDS) {
            setAxesBounds((Double[]) value);
        } else if (property == AxesProperty.HIDDENCOLOR) {
            setHiddenColor((Integer) value);
        } else if (property == LinePropertyType.MODE) {
            return setLineMode((Boolean) value);
        } else if (property == LinePropertyType.LINESTYLE) {
            setLineStyle((Integer) value);
        } else if (property == LinePropertyType.THICKNESS) {
            setLineThickness((Double) value);
        } else if (property == LinePropertyType.COLOR) {
            setLineColor((Integer) value);
        } else if (property == MarkPropertyType.MODE) {
            return setMarkMode((Boolean) value);
        } else if (property == MarkPropertyType.STYLE) {
            return setMarkStyle((Integer) value);
        } else if (property == MarkPropertyType.SIZE) {
            return setMarkSize((Integer) value);
        } else if (property == MarkPropertyType.SIZEUNIT) {
            setMarkSizeUnit((Integer) value);
        } else if (property == MarkPropertyType.FOREGROUND) {
            setMarkForeground((Integer) value);
        } else if (property == MarkPropertyType.BACKGROUND) {
            setMarkBackground((Integer) value);
        } else if (property == ClippablePropertyType.CLIPSTATE) {
            setClipState((Integer) value);
        } else if (property == ClippablePropertyType.CLIPBOX) {
            setClipBox((Double[]) value);
        } else if (property == ClippablePropertyType.CLIPBOXSET) {
            setClipBoxSet((Boolean) value);
        } else if (property == ArcProperty.ARCDRAWINGMETHOD) {
            setArcDrawingMethod((Integer) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the autoClear
     */
    public Boolean getAutoClear() {
        return autoClear;
    }

    /**
     * @param autoClear the autoClear to set
     */
    public void setAutoClear(Boolean autoClear) {
        this.autoClear = autoClear;
    }

    /**
     * @return the axes
     */
    public AxisProperty[] getAxes() {
        return axes;
    }

    /**
     * @param axes the axes to set
     */
    public void setAxes(AxisProperty[] axes) {
        this.axes = axes;
    }

    /**
     * @return the x axis
     */
    public AxisProperty getXAxis() {
        return axes[0];
    }

    /**
     * @param xAxis the x axis to set
     */
    public void setXAxis(AxisProperty xAxis) {
        axes[0] = xAxis;
    }

    /**
     * @return the x axis visible
     */
    public Boolean getXAxisVisible() {
        return axes[0].getVisible();
    }

    /**
     * @param visible the x axis visible to set
     */
    public void setXAxisVisible(Boolean visible) {
        axes[0].setVisible(visible);
    }

    /**
     * @return the x axis reverse
     */
    public Boolean getXAxisReverse() {
        return axes[0].getReverse();
    }

    /**
     * @param reverse the x axis reverse to set
     */
    public void setXAxisReverse(Boolean reverse) {
        axes[0].setReverse(reverse);
    }

    /**
     * @return the x axis grid color
     */
    public Integer getXAxisGridColor() {
        return axes[0].getGridColor();
    }

    /**
     * @param gridColor the x axis grid color to set
     */
    public void setXAxisGridColor(Integer gridColor) {
        axes[0].setGridColor(gridColor);
    }

    /**
     * @return the x axis label UID
     */
    public String getXAxisLabel() {
        return axes[0].getLabel();
    }

    /**
     * @param label the x axis label to set
     */
    public void setXAxisLabel(String label) {
        axes[0].setLabel(label);
    }

    /**
     * @return the x axis location
     */
    public Integer getXAxisLocation() {
        return getXAxisLocationAsEnum().ordinal();
    }

    /**
     * @return the x axis location
     */
    public AxisLocation getXAxisLocationAsEnum() {
        return axes[0].getAxisLocation();
    }

    /**
     * @param axisLocation the x axis location to set
     */
    public void setXAxisLocation(Integer axisLocation) {
        setXAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
    }

    /**
     * @param axisLocation the x axis location to set
     */
    public void setXAxisLocationAsEnum(AxisLocation axisLocation) {
        axes[0].setAxisLocation(axisLocation);
    }

    /**
     * @return the x axis log flag
     */
    public Boolean getXAxisLogFlag() {
        return axes[0].getLogFlag();
    }

    /**
     * @param logFlag the x axis log flag to set
     */
    public void setXAxisLogFlag(Boolean logFlag) {
        axes[0].setLogFlag(logFlag);
    }

    /**
     * @return the x axis ticks
     */
    public TicksProperty getXAxisTicks() {
        return axes[0].getTicks();
    }

    /**
     * @param ticks the x axis ticks to set
     */
    public void setXAxisTicks(TicksProperty ticks) {
        axes[0].setTicks(ticks);
    }

    /**
     * @return the x axis autoticks
     */
    public Boolean getXAxisAutoTicks() {
        return axes[0].getAutoTicks();
    }

    /**
     * @param autoTicks the x axis autoticks to set
     */
    public void setXAxisAutoTicks(Boolean autoTicks) {
        axes[0].setAutoTicks(autoTicks);
    }

    /**
     * @return the x axis number of ticks
     */
    public Integer getXAxisNumberTicks() {
        return axes[0].getNumberOfTicks();
    }

    /**
     * @return the x axis ticks locations
     */
    public Double[] getXAxisTicksLocations() {
        return axes[0].getTicksLocations();
    }

    /**
     * @param ticksLocations the x axis ticks locations to set
     */
    public void setXAxisTicksLocations(Double[] ticksLocations) {
        axes[0].setTicksLocations(ticksLocations);
    }

    /**
     * @return the x axis ticks labels
     */
    public String[] getXAxisTicksLabels() {
        return axes[0].getTicksLabelsStrings();
    }

    /**
     * @return the x axis ticks labels
     */
    public ArrayList<FormattedText> getXAxisTicksLabelsAsArrayList() {
        return axes[0].getTicksLabels();
    }

    /**
     * @param labels the x axis ticks labels to set
     */
    public void setXAxisTicksLabels(String[] labels) {
        axes[0].setTicksLabelsStrings(labels);
    }

    /**
     * @param labels the x axis ticks labels to set
     */
    public void setXAxisTicksLabelsAsArrayList(ArrayList<FormattedText> labels) {
        axes[0].setTicksLabels(labels);
    }

    /**
     * @return the x axis number of subticks
     */
    public Integer getXAxisSubticks() {
        return axes[0].getSubticks();
    }

    /**
     * @param subticks the x axis number of subticks to set
     */
    public void setXAxisSubticks(Integer subticks) {
        axes[0].setSubticks(subticks);
    }

    /**
     * @return the y axis
     */
    public AxisProperty getYAxis() {
        return axes[1];
    }

    /**
     * @param yAxis the y axis to set
     */
    public void setYAxis(AxisProperty yAxis) {
        axes[1] = yAxis;
    }

    /**
     * @return the y axis visible
     */
    public Boolean getYAxisVisible() {
        return axes[1].getVisible();
    }

    /**
     * @param visible the y axis visible to set
     */
    public void setYAxisVisible(Boolean visible) {
        axes[1].setVisible(visible);
    }

    /**
     * @return the y axis reverse
     */
    public Boolean getYAxisReverse() {
        return axes[1].getReverse();
    }

    /**
     * @param reverse the y axis reverse to set
     */
    public void setYAxisReverse(Boolean reverse) {
        axes[1].setReverse(reverse);
    }

    /**
     * @return the y axis grid color
     */
    public Integer getYAxisGridColor() {
        return axes[1].getGridColor();
    }

    /**
     * @param gridColor the y axis grid color to set
     */
    public void setYAxisGridColor(Integer gridColor) {
        axes[1].setGridColor(gridColor);
    }

    /**
     * @return the y axis label UID
     */
    public String getYAxisLabel() {
        return axes[1].getLabel();
    }

    /**
     * @param label the y axis label to set
     */
    public void setYAxisLabel(String label) {
        axes[1].setLabel(label);
    }

    /**
     * @return the y axis location
     */
    public Integer getYAxisLocation() {
        return getYAxisLocationAsEnum().ordinal();
    }

    /**
     * @return the y axis location
     */
    public AxisLocation getYAxisLocationAsEnum() {
        return axes[1].getAxisLocation();
    }

    /**
     * @param axisLocation the y axis location to set
     */
    public void setYAxisLocation(Integer axisLocation) {
        setYAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
    }

    /**
     * @param axisLocation the y axis location to set
     */
    public void setYAxisLocationAsEnum(AxisLocation axisLocation) {
        axes[1].setAxisLocation(axisLocation);
    }

    /**
     * @return the y axis log flag
     */
    public Boolean getYAxisLogFlag() {
        return axes[1].getLogFlag();
    }

    /**
     * @param logFlag the y axis log flag to set
     */
    public void setYAxisLogFlag(Boolean logFlag) {
        axes[1].setLogFlag(logFlag);
    }

    /**
     * @return the y axis ticks
     */
    public TicksProperty getYAxisTicks() {
        return axes[1].getTicks();
    }

    /**
     * @param ticks the y axis ticks to set
     */
    public void setYAxisTicks(TicksProperty ticks) {
        axes[1].setTicks(ticks);
    }

    /**
     * @return the y axis autoticks
     */
    public Boolean getYAxisAutoTicks() {
        return axes[1].getAutoTicks();
    }

    /**
     * @param autoTicks the y axis autoticks to set
     */
    public void setYAxisAutoTicks(Boolean autoTicks) {
        axes[1].setAutoTicks(autoTicks);
    }

    /**
     * @return the y axis number of ticks
     */
    public Integer getYAxisNumberTicks() {
        return axes[1].getNumberOfTicks();
    }

    /**
     * @return the y axis ticks locations
     */
    public Double[] getYAxisTicksLocations() {
        return axes[1].getTicksLocations();
    }

    /**
     * @param ticksLocations the y axis ticks locations to set
     */
    public void setYAxisTicksLocations(Double[] ticksLocations) {
        axes[1].setTicksLocations(ticksLocations);
    }

    /**
     * @return the y axis ticks labels
     */
    public String[] getYAxisTicksLabels() {
        return axes[1].getTicksLabelsStrings();
    }

    /**
     * @return the y axis ticks labels
     */
    public ArrayList<FormattedText> getYAxisTicksLabelsAsArrayList() {
        return axes[1].getTicksLabels();
    }

    /**
     * @param labels the y axis ticks labels to set
     */
    public void setYAxisTicksLabels(String[] labels) {
        axes[1].setTicksLabelsStrings(labels);
    }

    /**
     * @param labels the y axis ticks labels to set
     */
    public void setYAxisTicksLabelsAsArrayList(ArrayList<FormattedText> labels) {
        axes[1].setTicksLabels(labels);
    }

    /**
     * @return the y axis number of subticks
     */
    public Integer getYAxisSubticks() {
        return axes[1].getSubticks();
    }

    /**
     * @param subticks the y axis number of subticks to set
     */
    public void setYAxisSubticks(Integer subticks) {
        axes[1].setSubticks(subticks);
    }

    /**
     * @return the z axis
     */
    public AxisProperty getZAxis() {
        return axes[2];
    }

    /**
     * @param zAxis the z axis to set
     */
    public void setZAxis(AxisProperty zAxis) {
        axes[2] = zAxis;
    }

    /**
     * @return the z axis visible
     */
    public Boolean getZAxisVisible() {
        return axes[2].getVisible();
    }

    /**
     * @param visible the z axis visible to set
     */
    public void setZAxisVisible(Boolean visible) {
        axes[2].setVisible(visible);
    }

    /**
     * @return the z axis reverse
     */
    public Boolean getZAxisReverse() {
        return axes[2].getReverse();
    }

    /**
     * @param reverse the z axis reverse to set
     */
    public void setZAxisReverse(Boolean reverse) {
        axes[2].setReverse(reverse);
    }

    /**
     * @return the z axis grid color
     */
    public Integer getZAxisGridColor() {
        return axes[2].getGridColor();
    }

    /**
     * @param gridColor the z axis grid color to set
     */
    public void setZAxisGridColor(Integer gridColor) {
        axes[2].setGridColor(gridColor);
    }

    /**
     * @return the z axis label UID
     */
    public String getZAxisLabel() {
        return axes[2].getLabel();
    }

    /**
     * @param label the z axis label to set
     */
    public void setZAxisLabel(String label) {
        axes[2].setLabel(label);
    }

    /**
     * @return the z axis location
     */
    public Integer getZAxisLocation() {
        return getZAxisLocationAsEnum().ordinal();
    }

    /**
     * @return the z axis location
     */
    public AxisLocation getZAxisLocationAsEnum() {
        return axes[2].getAxisLocation();
    }

    /**
     * @param axisLocation the z axis location to set
     */
    public void setZAxisLocation(Integer axisLocation) {
        setZAxisLocationAsEnum(AxisLocation.intToEnum(axisLocation));
    }

    /**
     * @param axisLocation the z axis location to set
     */
    public void setZAxisLocationAsEnum(AxisLocation axisLocation) {
        axes[2].setAxisLocation(axisLocation);
    }

    /**
     * @return the z axis log flag
     */
    public Boolean getZAxisLogFlag() {
        return axes[2].getLogFlag();
    }

    /**
     * @param logFlag the z axis log flag to set
     */
    public void setZAxisLogFlag(Boolean logFlag) {
        axes[2].setLogFlag(logFlag);
    }

    /**
     * @return the z axis ticks
     */
    public TicksProperty getZAxisTicks() {
        return axes[2].getTicks();
    }

    /**
     * @param ticks the z axis ticks to set
     */
    public void setZAxisTicks(TicksProperty ticks) {
        axes[2].setTicks(ticks);
    }

    /**
     * @return the z axis autoticks
     */
    public Boolean getZAxisAutoTicks() {
        return axes[2].getAutoTicks();
    }

    /**
     * @param autoTicks the z axis autoticks to set
     */
    public void setZAxisAutoTicks(Boolean autoTicks) {
        axes[2].setAutoTicks(autoTicks);
    }

    /**
     * @return the z axis number of ticks
     */
    public Integer getZAxisNumberTicks() {
        return axes[2].getNumberOfTicks();
    }

    /**
     * @return the z axis ticks locations
     */
    public Double[] getZAxisTicksLocations() {
        return axes[2].getTicksLocations();
    }

    /**
     * @param ticksLocations the z axis ticks locations to set
     */
    public void setZAxisTicksLocations(Double[] ticksLocations) {
        axes[2].setTicksLocations(ticksLocations);
    }

    /**
     * @return the z axis ticks labels
     */
    public String[] getZAxisTicksLabels() {
        return axes[2].getTicksLabelsStrings();
    }

    /**
     * @return the z axis ticks labels
     */
    public ArrayList<FormattedText> getZAxisTicksLabelsAsArrayList() {
        return axes[2].getTicksLabels();
    }

    /**
     * @param labels the z axis ticks labels to set
     */
    public void setZAxisTicksLabels(String[] labels) {
        axes[2].setTicksLabelsStrings(labels);
    }

    /**
     * @param labels the z axis ticks labels to set
     */
    public void setZAxisTicksLabelsAsArrayList(ArrayList<FormattedText> labels) {
        axes[2].setTicksLabels(labels);
    }

    /**
     * @return the z axis number of subticks
     */
    public Integer getZAxisSubticks() {
        return axes[2].getSubticks();
    }

    /**
     * @param subticks the z axis number of subticks to set
     */
    public void setZAxisSubticks(Integer subticks) {
        axes[2].setSubticks(subticks);
    }

    /**
     * @return the autosubticks
     */
    public Boolean getAutoSubticks() {
        return autoSubticks;
    }

    /**
     * @param autoSubticks the autosubticks to set
     */
    public void setAutoSubticks(Boolean autoSubticks) {
        this.autoSubticks = autoSubticks;
    }

    /**
     * Gets the ticks labels font style.
     * It supposes all ticks labels within a single axis have the same font style value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @return the ticks labels font style
     */
    public Integer getFontStyle() {
        return axes[0].getFontStyle();
    }

    /**
     * Sets the ticks labels font style.
     * It supposes all ticks labels within a single axis have the same font style value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @param fontStyle the ticks labels font style to set
     */
    public void setFontStyle(Integer fontStyle) {
        for (int i = 0; i < axes.length; i++) {
            axes[i].setFontStyle(fontStyle);
        }
    }

    /**
     * Gets the ticks labels font size.
     * It supposes all ticks labels within a single axis have the same font size value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @return the ticks labels font size
     */
    public Double getFontSize() {
        return axes[0].getFontSize();
    }

    /**
     * Sets the ticks labels font size.
     * It supposes all ticks labels within a single axis have the same font size value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @param fontSize the ticks labels font size to set
     */
    public void setFontSize(Double fontSize) {
        for (int i = 0; i < axes.length; i++) {
            axes[i].setFontSize(fontSize);
        }
    }

    /**
     * Gets the ticks labels font color.
     * It supposes all ticks labels within a single axis have the same font color value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @return the ticks labels font color
     */
    public Integer getFontColor() {
        return axes[0].getFontColor();
    }

    /**
     * Sets the ticks labels font color.
     * It supposes all ticks labels within a single axis have the same font color value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @param fontColor the ticks labels font color to set
     */
    public void setFontColor(Integer fontColor) {
        for (int i = 0; i < axes.length; i++) {
            axes[i].setFontColor(fontColor);
        }
    }

    /**
     * Gets the ticks labels font fractional.
     * It supposes all ticks labels within a single axis have the same font fractional value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @return the ticks labels font fractional
     */
    public Boolean getFontFractional() {
        return axes[0].getFontFractional();
    }

    /**
     * Sets the ticks labels font fractional.
     * It supposes all ticks labels within a single axis have the same font fractional value
     * and that this value is the same for the 3 axes.
     * To be corrected.
     * @param fontFractional the ticks labels font fractional to set
     */
    public void setFontFractional(Boolean fontFractional) {
        for (int i = 0; i < axes.length; i++) {
            axes[i].setFontFractional(fontFractional);
        }
    }

    /**
     * @return the axesBounds
     */
    public Double[] getAxesBounds() {
        Double[] retAxesBounds = new Double[4];

        retAxesBounds[0] = axesBounds[0];
        retAxesBounds[1] = axesBounds[1];
        retAxesBounds[2] = axesBounds[2];
        retAxesBounds[3] = axesBounds[3];

        return retAxesBounds;
    }

    /**
     * @param axesBounds the axesBounds to set
     */
    public void setAxesBounds(Double[] axesBounds) {
        this.axesBounds[0] = axesBounds[0];
        this.axesBounds[1] = axesBounds[1];
        this.axesBounds[2] = axesBounds[2];
        this.axesBounds[3] = axesBounds[3];
    }

    /**
     * @return the hiddenColor
     */
    public Integer getHiddenColor() {
        return hiddenColor;
    }

    /**
     * @param hiddenColor the hiddenColor to set
     */
    public void setHiddenColor(Integer hiddenColor) {
        this.hiddenColor = hiddenColor;
    }

    /**
     * Returns the line property
     * @return the line property
     */
    public Line getLine() {
        return line;
    }

    /**
     * @return the line mode
     */
    public Boolean getLineMode() {
        return line.getMode();
    }

    /**
     * @param lineMode the line mode to set
     */
    public UpdateStatus setLineMode(Boolean lineMode) {
        return line.setMode(lineMode);
    }

    /**
     * @return the line style
     */
    public Integer getLineStyle() {
        return line.getLineStyle().asScilabIndex();
    }

    /**
     * @param lineStyle the line style to set
     */
    public void setLineStyle(Integer lineStyle) {
        line.setLineStyle(LineType.fromScilabIndex(lineStyle));
    }

    /**
     * @return the line thickness
     */
    public Double getLineThickness() {
        return line.getThickness();
    }

    /**
     * @param lineThickness the line thickness to set
     */
    public void setLineThickness(Double lineThickness) {
        line.setThickness(lineThickness);
    }

    /**
     * @return the line color
     */
    public Integer getLineColor() {
        return line.getColor();
    }

    /**
     * @param lineColor the lineColor to set
     */
    public UpdateStatus setLineColor(Integer lineColor) {
        return line.setColor(lineColor);
    }

    /**
     * @return the mark mode
     */
    public Boolean getMarkMode() {
        return mark.getMode();
    }

    /**
     * @param markMode the mark mode to set
     */
    public UpdateStatus setMarkMode(Boolean markMode) {
        return mark.setMode(markMode);
    }

    /**
     * @return the mark style
     */
    public Integer getMarkStyle() {
        return mark.getStyle();
    }

    /**
     * @param markStyle the mark style to set
     */
    public UpdateStatus setMarkStyle(Integer markStyle) {
        return mark.setStyle(markStyle);
    }

    /**
     * @return the mark size
     */
    public Integer getMarkSize() {
        return mark.getSize();
    }

    /**
     * @param markSize the mark size to set
     */
    public UpdateStatus setMarkSize(Integer markSize) {
        return mark.setSize(markSize);
    }

    /**
     * @return the mark size unit
     */
    public Integer getMarkSizeUnit() {
        return mark.getMarkSizeUnit().ordinal();
    }

    /**
     * @param markSizeUnit the mark size unit to set
     */
    public void setMarkSizeUnit(Integer markSizeUnit) {
        mark.setMarkSizeUnit(MarkSizeUnitType.intToEnum(markSizeUnit));
    }

    /**
     * @return the mark foreground
     */
    public Integer getMarkForeground() {
        return mark.getForeground();
    }

    /**
     * @param markForeground the mark foreground to set
     */
    public void setMarkForeground(Integer markForeground) {
        mark.setForeground(markForeground);
    }

    /**
     * @return the mark background
     */
    public Integer getMarkBackground() {
        return mark.getBackground();
    }

    /**
     * @param markBackground the mark background to set
     */
    public void setMarkBackground(Integer markBackground) {
        mark.setBackground(markBackground);
    }

    /**
     * @return the clip state
     */
    public Integer getClipState() {
        return clipProperty.getClipState().ordinal();
    }

    /**
     * @param clipState the clip state to set
     */
    public void setClipState(Integer clipState) {
        clipProperty.setClipState(ClipStateType.intToEnum(clipState));
    }

    /**
     * @return the clip box
     */
    public Double[] getClipBox() {
        return clipProperty.getClipBox();
    }

    /**
     * @param clipBox the clip box to set
     */
    public void setClipBox(Double[] clipBox) {
        clipProperty.setClipBox(clipBox);
    }

    /**
     * @return the clip box set
     */
    public Boolean getClipBoxSet() {
        return clipProperty.getClipBoxSet();
    }

    /**
     * @param clipBoxSet the clip box set to set
     */
    public void setClipBoxSet(Boolean clipBoxSet) {
        clipProperty.setClipBoxSet(clipBoxSet);
    }

    /**
     * @return the arcDrawingMethod
     */
    public Integer getArcDrawingMethod() {
        return arcDrawingMethod.ordinal();
    }

    /**
     * @param arcDrawingMethod the arcDrawingMethod to set
     */
    public void setArcDrawingMethod(Integer arcDrawingMethod) {
        this.arcDrawingMethod = ArcDrawingMethod.intToEnum(arcDrawingMethod);
    }

    /**
     * @return the box
     */
    public Box getBox() {
        return box;
    }

    /**
     * @param box the box to set
     */
    public void setBox(Box box) {
        this.box = box;
    }

    /**
     * @return the box type
     */
    public Integer getBoxType() {
        return getBoxTypeAsEnum().ordinal();
    }

    /**
     * @return the box type
     */
    public BoxType getBoxTypeAsEnum() {
        return box.getBox();
    }

    /**
     * @param box the BoxType to set
     */
    public void setBoxType(Integer box) {
        setBoxTypeAsEnum(BoxType.intToEnum(box));
    }

    /**
     * @param box the BoxType to set
     */
    public void setBoxTypeAsEnum(BoxType box) {
        this.box.setBox(box);
    }

    /**
     * @return the hidden axis color
     */
    public Integer getHiddenAxisColor() {
        return box.getHiddenAxisColor();
    }

    /**
     * @param color the hidden axis color to set
     */
    public void setHiddenAxisColor(Integer color) {
        box.setHiddenAxisColor(color);
    }

    /**
     * @return the tight limits
     */
    public Boolean getTightLimits() {
        return box.getTightLimits();
    }

    /**
     * @param tightLimits the tightLimits to set
     */
    public void setTightLimits(Boolean tightLimits) {
        box.setTightLimits(tightLimits);
    }

    /**
     * @return the data bounds
     */
    public Double[] getDataBounds() {
        return box.getDataBounds();
    }

    /**
     * @param dataBounds the data bounds to set
     */
    public void setDataBounds(Double[] dataBounds) {
        box.setDataBounds(dataBounds);
    }

    /**
     * @return the real data bounds
     */
    public Double[] getRealDataBounds() {
        return box.getRealDataBounds();
    }

    /**
     * @param realDataBounds the real data bounds to set
     */
    public void setRealDataBounds(Double[] realDataBounds) {
        box.setRealDataBounds(realDataBounds);
    }

    /**
     * Current displayed bounds getter.
     * @return the current visible bounds of this axes.
     */
    public Double[] getDisplayedBounds() {
        if (getZoomEnabled()) {
            return getZoomBox();
        } else {
            return getMaximalDisplayedBounds();
        }
    }

    /**
     * Maximal possible displayed bounds getter.
     * @return the maximal possible displayed bounds.
     */
    public Double[] getMaximalDisplayedBounds() {
        Double[] bounds = getDataBounds();

        /**
         * Remove 0 sized bounds
         */
        for (int i = 0 ; i < 6 ; i += 2) {
            if (bounds[i].equals(bounds[i + 1])) {
                bounds[i]--;
                bounds[i + 1]++;
            }
        }

        if (getXAxisLogFlag()) {
            bounds[0] = Math.log10(bounds[0]);
            bounds[1] = Math.log10(bounds[1]);
        }

        if (getYAxisLogFlag()) {
            bounds[2] = Math.log10(bounds[2]);
            bounds[3] = Math.log10(bounds[3]);
        }

        if (getZAxisLogFlag()) {
            bounds[4] = Math.log10(bounds[4]);
            bounds[5] = Math.log10(bounds[5]);
        }

        if (!getTightLimits()) {
            for (int i = 0 ; i < 6 ; i += 2) {
                round(bounds, i);
            }
        }

        return bounds;
    }

    /**
     * Round the bounds in the bounds array at the given index.
     * bounds[i] and bounds[i + 1 ] are rounded to be in the value written
     * k * b * 10^n
     * where b is in {1, 2, 5}
     * and b * 10 ^n the maximal value less than (bounds[i + 1] - bounds[i]) / BOUNDS_PARAMETER.
     * @param bounds the bounds array.
     * @param i the start index.
     */
    private void round(Double[] bounds, int i) {
        double delta = (bounds[i + 1] - bounds[i]) / BOUNDS_PARAMETER;
        double powerOfTen = Math.pow(10, Math.floor(Math.log10(delta)));
        double base = delta / powerOfTen;

        if (base < 2) {
            base = 1;
        } else if (base < 5) {
            base = 2;
        } else {
            base = 5;
        }

        double step = base * powerOfTen;
        bounds[i] = step * Math.floor(bounds[i] / step);
        bounds[i + 1] = step * Math.ceil(bounds[i + 1] / step);
    }

    /**
     * @return the zoomEnabled
     */
    public Boolean getZoomEnabled() {
        return box.getZoomEnabled();
    }

    /**
     * @param zoomEnabled the zoomEnabled to set
     */
    public void setZoomEnabled(Boolean zoomEnabled) {
        box.setZoomEnabled(zoomEnabled);
    }

    /**
     * @return the zoom box
     */
    public Double[] getZoomBox() {
        return box.getZoomBox();
    }

    /**
     * @param zoomBox the zoom box to set
     */
    public void setZoomBox(Double[] zoomBox) {
        box.setZoomBox(zoomBox);
    }

    /**
     * @return the autoscale
     */
    public Boolean getAutoScale() {
        return box.getAutoScale();
    }

    /**
     * @param autoScale the autoscale to set
     */
    public void setAutoScale(Boolean autoScale) {
        box.setAutoScale(autoScale);
    }

    /**
     * @return the firstplot
     */
    public Boolean getFirstPlot() {
        return box.getFirstPlot();
    }

    /**
     * @param firstPlot the firstplot to set
     */
    public void setFirstPlot(Boolean firstPlot) {
        box.setFirstPlot(firstPlot);
    }

    /**
     * @return the camera
     */
    public Camera getCamera() {
        return camera;
    }

    /**
     * @param camera the camera to set
     */
    public void setCamera(Camera camera) {
        this.camera = camera;
    }

    /**
     * @return the view type
     */
    public Integer getView() {
        return getViewAsEnum().ordinal();
    }

    /**
     * @return the view type
     */
    public ViewType getViewAsEnum() {
        return camera.getView();
    }

    /**
     * @param view the view type to set
     */
    public void setView(Integer view) {
        setViewAsEnum(ViewType.intToEnum(view));
    }

    /**
     * @param view the view type to set
     */
    public void setViewAsEnum(ViewType view) {
        camera.setView(view);
    }

    /**
     * @return the isoview
     */
    public Boolean getIsoview() {
        return camera.getIsoview();
    }

    /**
     * @param isoview the isoview to set
     */
    public void setIsoview(Boolean isoview) {
        camera.setIsoview(isoview);
    }

    /**
     * @return the cubescaling
     */
    public Boolean getCubeScaling() {
        return camera.getCubeScaling();
    }

    /**
     * @param cubeScaling the cubescaling to set
     */
    public void setCubeScaling(Boolean cubeScaling) {
        camera.setCubeScaling(cubeScaling);
    }

    /**
     * @return the rotation angles
     */
    public Double[] getRotationAngles() {
        return camera.getRotationAngles();
    }

    /**
     * @param rotationAngles the rotation angles to set
     * @return the update status.
     */
    public UpdateStatus setRotationAngles(Double[] rotationAngles) {
        if (camera.setRotationAngles(rotationAngles)) {
            try {
                GraphicController controller = GraphicController.getController();
                Figure figure = (Figure) controller.getObjectFromId(getParentFigure());
                if (figure.getRotationAsEnum().equals(Figure.RotationType.MULTIPLE)) {
                    for (String child : figure.getChildren()) {
                        if (child != null) {
                            if (GraphicObjectProperties.__GO_AXES__ == ((Integer) controller.getProperty(child, GraphicObjectProperties.__GO_TYPE__))) {
                                controller.setProperty(
                                    child,
                                    GraphicObjectProperties.__GO_ROTATION_ANGLES__,
                                    rotationAngles);
                            }
                        }
                    }
                }
            } catch (ClassCastException ignored) {
            } catch (NullPointerException ignored) {
            }
            return UpdateStatus.Success;
        } else {
            return UpdateStatus.NoChange;
        }
    }

    /**
     * @return the 3d rotation angles
     */
    public Double[] getRotationAngles3d() {
        return camera.getRotationAngles3d();
    }

    /**
     * @param rotationAngles3d the 3d rotation angles to set
     */
    public void setRotationAngles3d(Double[] rotationAngles3d) {
        camera.setRotationAngles3d(rotationAngles3d);
    }

    /**
     * @return the filled
     */
    public Boolean getFilled() {
        return filled;
    }

    /**
     * @param filled the filled to set
     */
    public void setFilled(Boolean filled) {
        this.filled = filled;
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
     * @return the gridPosition
     */
    public Integer getGridPosition() {
        return getGridPositionAsEnum().ordinal();
    }

    /**
     * @return the gridPosition
     */
    public GridPosition getGridPositionAsEnum() {
        return gridPosition;
    }

    /**
     * @param gridPosition the gridPosition to set
     */
    public void setGridPosition(Integer gridPosition) {
        setGridPositionAsEnum(GridPosition.intToEnum(gridPosition));
    }

    /**
     * @param gridPosition the gridPosition to set
     */
    public void setGridPositionAsEnum(GridPosition gridPosition) {
        this.gridPosition = gridPosition;
    }

    /**
     * @return the margins
     */
    public Double[] getMargins() {
        Double[] retMargins = new Double[4];

        retMargins[0] = margins[0];
        retMargins[1] = margins[1];
        retMargins[2] = margins[2];
        retMargins[3] = margins[3];

        return retMargins;
    }

    /**
     * @param margins the margins to set
     */
    public void setMargins(Double[] margins) {
        this.margins[0] = margins[0];
        this.margins[1] = margins[1];
        this.margins[2] = margins[2];
        this.margins[3] = margins[3];
    }

    /**
     * @return the title UID
     */
    public String getTitle() {
        return title;
    }

    /**
     * @param title the title to set
     */
    public void setTitle(String title) {
        this.title = title;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_AXES__;
    }

}
