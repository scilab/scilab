/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.graphic_objects.axes;

import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.textObject.FormattedText;

import java.util.ArrayList;


/**
 * Axis property class
 * @author Manuel JULIACHS
 */
public class AxisProperty {
    /**
     * AxisProperty properties names
     */
    public enum AxisPropertyProperty { VISIBLE, REVERSE, GRIDCOLOR, GRIDTHICKNESS, GRIDSTYLE, LABEL, AXISLOCATION,
                                       LOGFLAG, UNKNOWNPROPERTY
                                     }

    /**
     * Axis location type
     * BOTTOM, TOP, MIDDLE, ORIGIN are allowed for an x-axis,
     * LEFT, RIGHT, MIDDLE or ORIGIN are allowed for a y-axis
     */
    public static enum AxisLocation { BOTTOM, TOP, MIDDLE, ORIGIN, LEFT, RIGHT;


                                      /**
                                       * Converts an integer to the corresponding enum
                                       * @param intValue the integer value
                                       * @return the axis location enum
                                       */
    public static AxisLocation intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return AxisLocation.BOTTOM;
            case 1:
                return AxisLocation.TOP;
            case 2:
                return AxisLocation.MIDDLE;
            case 3:
                return AxisLocation.ORIGIN;
            case 4:
                return AxisLocation.LEFT;
            case 5:
                return AxisLocation.RIGHT;
            default:
                return null;
        }
    }
                                    }

    /** Specifies whether the axis is visible or not */
    private boolean visible;

    /** Specifies whether the axis is reversed or not */
    private boolean reverse;

    /** Grid color */
    private int gridColor;

    /** Grid thickness */
    private double gridThickness;

    /** Grid style */
    private Line.LineType gridStyle;

    /** Axis label UID */
    private Integer label;

    /** Axis location */
    private AxisLocation axisLocation;

    /** Specifies whether logarithmic coordinates are used or not */
    private boolean logFlag;

    /** Ticks property */
    private TicksProperty ticks;

    /** Constructor */
    public AxisProperty() {
        visible = false;
        reverse = false;
        gridColor = 0;
        gridThickness = -1;
        gridStyle = Line.LineType.DASH_DOT;

        /* Sets the label to the null object */
        label = 0;

        axisLocation = AxisLocation.ORIGIN;
        ticks = new TicksProperty();
        logFlag = false;
    }

    /**
     * Copy constructor
     * @param axisProperty the AxisProperty to copy
     */
    public AxisProperty(AxisProperty axisProperty) {
        visible = axisProperty.visible;
        reverse = axisProperty.reverse;
        gridColor = axisProperty.gridColor;
        gridThickness = axisProperty.gridThickness;
        gridStyle = axisProperty.gridStyle;

        label = 0;

        axisLocation = axisProperty.axisLocation;
        ticks = new TicksProperty(axisProperty.ticks);
        logFlag = axisProperty.logFlag;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals("Visible")) {
            return AxisPropertyProperty.VISIBLE;
        } else if (propertyName.equals("Reverse")) {
            return AxisPropertyProperty.REVERSE;
        } else if (propertyName.equals("GridColor")) {
            return AxisPropertyProperty.GRIDCOLOR;
        } else if (propertyName.equals("GridThickness")) {
            return AxisPropertyProperty.GRIDTHICKNESS;
        } else if (propertyName.equals("GridStyle")) {
            return AxisPropertyProperty.GRIDSTYLE;
        } else if (propertyName.equals("Label")) {
            return AxisPropertyProperty.LABEL;
        } else if (propertyName.equals("AxisLocation")) {
            return AxisPropertyProperty.AXISLOCATION;
        } else if (propertyName.equals("LogFlag")) {
            return AxisPropertyProperty.LOGFLAG;
        } else {
            return AxisPropertyProperty.UNKNOWNPROPERTY;
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == AxisPropertyProperty.VISIBLE) {
            return getVisible();
        } else if (property == AxisPropertyProperty.REVERSE) {
            return getReverse();
        } else if (property == AxisPropertyProperty.GRIDCOLOR) {
            return getGridColor();
        } else if (property == AxisPropertyProperty.GRIDTHICKNESS) {
            return getGridThickness();
        } else if (property == AxisPropertyProperty.GRIDSTYLE) {
            return getGridStyle();
        } else if (property == AxisPropertyProperty.LABEL) {
            return getLabel();
        } else if (property == AxisPropertyProperty.AXISLOCATION) {
            return getAxisLocation();
        } else if (property == AxisPropertyProperty.LOGFLAG) {
            return getLogFlag();
        } else {
            return null;
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public boolean setProperty(Object property, Object value) {
        if (property == AxisPropertyProperty.VISIBLE) {
            setVisible((Boolean) value);
        } else if (property == AxisPropertyProperty.REVERSE) {
            setReverse((Boolean) value);
        } else if (property == AxisPropertyProperty.GRIDCOLOR) {
            setGridColor((Integer) value);
        } else if (property == AxisPropertyProperty.GRIDTHICKNESS) {
            setGridThickness((Double) value);
        } else if (property == AxisPropertyProperty.GRIDSTYLE) {
            setGridStyle((Integer) value);
        } else if (property == AxisPropertyProperty.LABEL) {
            setLabel((Integer) value);
        } else if (property == AxisPropertyProperty.AXISLOCATION) {
            setAxisLocation((AxisLocation) value);
        } else if (property == AxisPropertyProperty.LOGFLAG) {
            setLogFlag((Boolean) value);
        }

        return true;
    }

    /**
     * @return the axisLocation
     */
    public AxisLocation getAxisLocationAsEnum() {
        return axisLocation;
    }

    /**
     * @return the axisLocation
     */
    public AxisLocation getAxisLocation() {
        return axisLocation;
    }

    /**
     * @param axisLocation the axisLocation to set
     */
    public UpdateStatus setAxisLocation(AxisLocation axisLocation) {
        if (this.axisLocation != axisLocation) {
            this.axisLocation = axisLocation;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the gridColor
     */
    public Integer getGridColor() {
        return gridColor;
    }

    /**
     * @param gridColor the gridColor to set
     */
    public UpdateStatus setGridColor(Integer gridColor) {
        if (this.gridColor != gridColor) {
            this.gridColor = gridColor;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the gridThickness
     */
    public Double getGridThickness() {
        return gridThickness;
    }

    /**
     * @param gridThickness the gridThickness to set
     */
    public UpdateStatus setGridThickness(Double gridThickness) {
        if (this.gridThickness != gridThickness) {
            this.gridThickness = gridThickness;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the gridStyle
     */
    public Integer getGridStyle() {
        return gridStyle.asScilabIndex();
    }

    /**
     * @param gridStyle the gridStyle to set
     */
    public UpdateStatus setGridStyle(Integer gridStyle) {
        Line.LineType type = Line.LineType.fromScilabIndex(gridStyle);
        if (this.gridStyle != type) {
            this.gridStyle = type;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the label
     */
    public Integer getLabel() {
        return label;
    }

    /**
     * @param label the label to set
     */
    public UpdateStatus setLabel(Integer label) {
        if (this.label != label) {
            this.label = label == null ? 0 : label;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the logFlag
     */
    public Boolean getLogFlag() {
        return logFlag;
    }

    /**
     * @param logFlag the logFlag to set
     */
    public UpdateStatus setLogFlag(Boolean logFlag) {
        if (this.logFlag != logFlag) {
            this.logFlag = logFlag;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the reverse
     */
    public Boolean getReverse() {
        return reverse;
    }

    /**
     * @param reverse the reverse to set
     */
    public UpdateStatus setReverse(Boolean reverse) {
        if (this.reverse != reverse) {
            this.reverse = reverse;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the ticks
     */
    public TicksProperty getTicks() {
        return ticks;
    }

    /**
     * @param ticks the ticks to set
     */
    public UpdateStatus setTicks(TicksProperty ticks) {
        if (!this.ticks.equals(ticks)) {
            this.ticks = ticks;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the visible
     */
    public Boolean getVisible() {
        return visible;
    }

    /**
     * @param visible the visible to set
     */
    public UpdateStatus setVisible(Boolean visible) {
        if (this.visible != visible) {
            this.visible = visible;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the autoticks
     */
    public Boolean getAutoTicks() {
        return ticks.getAuto();
    }

    /**
     * @param autoticks the autoticks to set
     */
    public UpdateStatus setAutoTicks(Boolean autoticks) {
        return ticks.setAuto(autoticks);
    }

    /**
     * @return the number of ticks
     */
    public Integer getNumberOfTicks() {
        return ticks.getNumber();
    }

    /**
     * @return the ticks locations
     */
    public Double[] getTicksLocations() {
        return ticks.getLocations();
    }

    /**
     * @param ticksLocations the ticks locations to set
     */
    public UpdateStatus setTicksLocations(Double[] ticksLocations) {
        return ticks.setLocations(ticksLocations);
    }

    /**
     * @return the ticks labels
     */
    public ArrayList<FormattedText> getTicksLabels() {
        return ticks.getLabels();
    }

    /**
     * @param labels the labels to set
     */
    public UpdateStatus setTicksLabels(ArrayList<FormattedText> labels) {
        return ticks.setLabels(labels);
    }

    /**
     * @return the ticks labels
     */
    public String[] getTicksLabelsStrings() {
        return ticks.getLabelsStrings();
    }

    /**
     * @param labels the labels strings to set
     */
    public UpdateStatus setTicksLabelsStrings(String[] labels) {
        return ticks.setLabelsStrings(labels);
    }

    /**
     * @return the number of subticks
     */
    public Integer getSubticks() {
        return ticks.getSubticks();
    }

    /**
     * @param subticks the number of subticks to set
     */
    public UpdateStatus setSubticks(Integer subticks) {
        return ticks.setSubticks(subticks);
    }

    /**
     * Supposes all ticks labels have the same font style.
     * To be corrected.
     * @return the ticks labels font style
     */
    public Integer getFontStyle() {
        return ticks.getFontStyle();
    }

    /**
     * Supposes all ticks labels have the same font style.
     * To be corrected.
     * @param fontStyle the ticks labels font style to set
     */
    public UpdateStatus setFontStyle(Integer fontStyle) {
        return ticks.setFontStyle(fontStyle);
    }

    /**
     * Supposes all ticks labels have the same font size.
     * To be corrected.
     * @return the ticks labels font size
     */
    public Double getFontSize() {
        return ticks.getFontSize();
    }

    /**
     * Supposes all ticks labels have the same font size.
     * To be corrected.
     * @param fontSize the ticks labels font size to set
     */
    public UpdateStatus setFontSize(Double fontSize) {
        return ticks.setFontSize(fontSize);
    }

    /**
     * @return the ticks labels format
     */
    public String getFormat() {
        return ticks.getFormat();
    }

    /**
     * @param format the ticks labels format set
     */
    public UpdateStatus setFormat(String format) {
        return ticks.setFormat(format);
    }

    /**
     * @return the ticks labels scale-translate factors
     */
    public Double[] getSTFactors() {
        return ticks.getSTFactors();
    }

    /**
     * @param factors the ticks labels scale-translate factors
     */
    public UpdateStatus setSTFactors(Double[] factors) {
        return ticks.setSTFactors(factors);
    }

    /**
     * Supposes all ticks labels have the same font color.
     * To be corrected.
     * @return the ticks labels font color
     */
    public Integer getFontColor() {
        return ticks.getFontColor();
    }

    /**
     * Supposes all ticks labels have the same font color.
     * To be corrected.
     * @param fontColor the ticks labels font color to set
     */
    public UpdateStatus setFontColor(Integer fontColor) {
        return ticks.setFontColor(fontColor);
    }

    /**
     * Supposes all ticks labels have the same font fractional.
     * To be corrected.
     * @return the ticks labels font fractional
     */
    public Boolean getFontFractional() {
        return ticks.getFontFractional();
    }

    /**
     * Supposes all ticks labels have the same font fractional.
     * To be corrected.
     * @param fontFractional the ticks labels font fractional to set
     */
    public UpdateStatus setFontFractional(Boolean fontFractional) {
        return ticks.setFontFractional(fontFractional);
    }
}
