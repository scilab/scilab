/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.axis;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_FRACTIONAL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FORMATN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NUMBER_TICKS_LABELS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SUBTICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_DIRECTION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_LABELS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_SEGMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TICKS_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_X_NUMBER_TICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_X_TICKS_COORDS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Y_NUMBER_TICKS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Y_TICKS_COORDS__;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.utils.TicksDirection;
import org.scilab.modules.graphic_objects.utils.TicksStyle;

/**
 * Axis class
 * @author Manuel JULIACHS
 */
public class Axis extends ClippableContouredObject {
    /** Axis properties */
    private enum AxisProperty { TICKSDIRECTION, XNUMBERTICKS, YNUMBERTICKS, XTICKSCOORDS, YTICKSCOORDS, TICKSCOLOR, TICKSSEGMENT, TICKSSTYLE,
                                SUBTICKS, NUMBERTICKSLABELS, TICKSLABELS, FORMATN, FONT
                              };

    /** Default number of ticks */
    private static final int DEFAULT_NUMBER_OF_TICKS = 10;

    /** Ticks direction */
    private TicksDirection ticksDirection;

    /** Ticks x-coordinate position vector */
    private double[] xTicksCoords;

    /** Ticks y-coordinate position vector */
    private double[] yTicksCoords;

    /** Ticks color */
    private int ticksColor;

    /** Specifies whether the axis segment is drawn */
    private boolean ticksSegment;

    /** Specifies the ticks style (either 0, 1, or 2) */
    private TicksStyle ticksStyle;

    /** Number of subticks */
    private int subticks;

    /** Ticks labels list */
    private ArrayList <String> ticksLabels;

    /** Label format */
    private String formatn;

    /** Font */
    private Font font;

    /** Constructor */
    public Axis() {
        super();
        ticksDirection = TicksDirection.TOP;
        xTicksCoords = new double[DEFAULT_NUMBER_OF_TICKS];
        yTicksCoords = new double[1];
        ticksColor = 0;
        ticksSegment = false;
        ticksLabels = new ArrayList<String>(DEFAULT_NUMBER_OF_TICKS);
        formatn = new String("");
        font = new Font();
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
            case __GO_TICKS_DIRECTION__ :
                return AxisProperty.TICKSDIRECTION;
            case __GO_X_NUMBER_TICKS__ :
                return AxisProperty.XNUMBERTICKS;
            case __GO_Y_NUMBER_TICKS__ :
                return AxisProperty.YNUMBERTICKS;
            case __GO_X_TICKS_COORDS__ :
                return AxisProperty.XTICKSCOORDS;
            case __GO_Y_TICKS_COORDS__ :
                return AxisProperty.YTICKSCOORDS;
            case __GO_TICKS_COLOR__ :
                return AxisProperty.TICKSCOLOR;
            case __GO_TICKS_SEGMENT__ :
                return AxisProperty.TICKSSEGMENT;
            case __GO_TICKS_STYLE__ :
                return AxisProperty.TICKSSTYLE;
            case __GO_SUBTICKS__ :
                return AxisProperty.SUBTICKS;
            case __GO_NUMBER_TICKS_LABELS__ :
                return AxisProperty.NUMBERTICKSLABELS;
            case __GO_TICKS_LABELS__ :
                return AxisProperty.TICKSLABELS;
            case __GO_FORMATN__ :
                return AxisProperty.FORMATN;
            case __GO_FONT__ :
                return AxisProperty.FONT;
            case __GO_FONT_STYLE__ :
                return Font.FontProperty.STYLE;
            case __GO_FONT_SIZE__ :
                return Font.FontProperty.SIZE;
            case __GO_FONT_COLOR__ :
                return Font.FontProperty.COLOR;
            case __GO_FONT_FRACTIONAL__ :
                return Font.FontProperty.FRACTIONAL;
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
        if (property == AxisProperty.TICKSDIRECTION) {
            return TicksDirection.enumToInt(getTicsDirection());
        } else if (property == AxisProperty.XNUMBERTICKS) {
            return getXNumberTics();
        } else if (property == AxisProperty.YNUMBERTICKS) {
            return getYNumberTics();
        } else if (property == AxisProperty.XTICKSCOORDS) {
            return getXTicsCoord();
        } else if (property == AxisProperty.YTICKSCOORDS) {
            return getYTicsCoord();
        } else if (property == AxisProperty.TICKSCOLOR) {
            return getTicsColor();
        } else if (property == AxisProperty.TICKSSEGMENT) {
            return getTicsSegment();
        } else if (property == AxisProperty.TICKSSTYLE) {
            return TicksStyle.enumToInt(getTicsStyle());
        } else if (property == AxisProperty.SUBTICKS) {
            return getSubtics();
        } else if (property == AxisProperty.NUMBERTICKSLABELS) {
            return getNumberTickLabels();
        } else if (property == AxisProperty.TICKSLABELS) {
            return getTicsLabels();
        } else if (property == AxisProperty.FORMATN) {
            return getFormatn();
        } else if (property == AxisProperty.FONT) {
            return getFont();
        } else if (property == Font.FontProperty.STYLE) {
            return getStyle();
        } else if (property == Font.FontProperty.SIZE) {
            return getLabelsFontSize();
        } else if (property == Font.FontProperty.COLOR) {
            return getLabelsFontColor();
        } else if (property == Font.FontProperty.FRACTIONAL) {
            return getFractionalFont();
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
        if (property == AxisProperty.TICKSDIRECTION) {
            setTicsDirection(TicksDirection.intToEnum((Integer) value));
        } else if (property == AxisProperty.XTICKSCOORDS) {
            setXTicsCoord((double[]) value);
        } else if (property == AxisProperty.YTICKSCOORDS) {
            setYTicsCoord((double[]) value);
        } else if (property == AxisProperty.TICKSCOLOR) {
            setTicsColor((Integer) value);
        } else if (property == AxisProperty.TICKSSEGMENT) {
            setTicsSegment((Boolean) value);
        } else if (property == AxisProperty.TICKSSTYLE) {
            setTicsStyle(TicksStyle.intToEnum((Integer) value));
        } else if (property == AxisProperty.SUBTICKS) {
            setSubtics((Integer) value);
        } else if (property == AxisProperty.TICKSLABELS) {
            setTicsLabels((String[]) value);
        } else if (property == AxisProperty.FORMATN) {
            setFormatn((String) value);
        } else if (property == AxisProperty.FONT) {
            setFont((Font) value);
        } else if (property == Font.FontProperty.STYLE) {
            setStyle((Integer) value);
        } else if (property == Font.FontProperty.SIZE) {
            setLabelsFontSize((Double) value);
        } else if (property == Font.FontProperty.COLOR) {
            setLabelsFontColor((Integer) value);
        } else if (property == Font.FontProperty.FRACTIONAL) {
            setFractionalFont((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the font
     */
    public Font getFont() {
        return font;
    }

    /**
     * @param font the font to set
     */
    public UpdateStatus setFont(Font font) {
        this.font = font;
        return UpdateStatus.Success;
    }

    /**
     * @return the font style
     */
    public Integer getStyle() {
        return font.getStyle();
    }

    /**
     * @param style the font style to set
     */
    public UpdateStatus setStyle(int style) {
        font.setStyle(style);
        return UpdateStatus.Success;
    }

    /**
     * @return the font size
     */
    public Double getLabelsFontSize() {
        return font.getSize();
    }

    /**
     * @param size the font size to set
     */
    public UpdateStatus setLabelsFontSize(double size) {
        font.setSize(size);
        return UpdateStatus.Success;
    }

    /**
     * @return the font color
     */
    public Integer getLabelsFontColor() {
        return font.getColor();
    }

    /**
     * @param color the font color to set
     */
    public UpdateStatus setLabelsFontColor(int color) {
        font.setColor(color);
        return UpdateStatus.Success;
    }

    /**
     * @return the font fractional
     */
    public Boolean getFractionalFont() {
        return font.getFractional();
    }

    /**
     * @param fractional the font fractional to set
     */
    public UpdateStatus setFractionalFont(boolean fractional) {
        font.setFractional(fractional);
        return UpdateStatus.Success;
    }

    /**
     * @return the formatn
     */
    public String getFormatn() {
        return formatn;
    }

    /**
     * @param formatn the formatn to set
     */
    public UpdateStatus setFormatn(String formatn) {
        this.formatn = formatn;
        return UpdateStatus.Success;
    }

    /**
     * @return the ticksColor
     */
    public Integer getTicsColor() {
        return ticksColor;
    }

    /**
     * @param ticksColor the ticksColor to set
     */
    public UpdateStatus setTicsColor(int ticksColor) {
        this.ticksColor = ticksColor;
        return UpdateStatus.Success;
    }

    /**
     * @return the ticksDirection
     */
    public TicksDirection getTicsDirection() {
        return ticksDirection;
    }

    /**
     * @param ticksDirection the ticksDirection to set
     */
    public UpdateStatus setTicsDirection(TicksDirection ticksDirection) {
        this.ticksDirection = ticksDirection;
        return UpdateStatus.Success;
    }

    /**
     * @return the number of ticks labels
     */
    public Integer getNumberTickLabels() {
        return ticksLabels.size();
    }

    /**
     * @return the ticksLabels
     */
    public String[] getTicsLabels() {
        String[] labels = new String[ticksLabels.size()];

        for (int i = 0; i < ticksLabels.size(); i++) {
            labels[i] = new String(ticksLabels.get(i));
        }

        return labels;
    }

    /**
     * @return the ticksLabels
     */
    public ArrayList<String> getTicksLabelsAsArrayList() {
        return ticksLabels;
    }

    /**
     * @param ticksLabels the ticksLabels to set
     */
    public UpdateStatus setTicsLabels(String[] ticksLabels) {
        if (!this.ticksLabels.isEmpty()) {
            this.ticksLabels.clear();
        }

        for (int i = 0; i < ticksLabels.length; i++) {
            this.ticksLabels.add(new String(ticksLabels[i]));
        }
        return UpdateStatus.Success;
    }

    /**
     * @param ticksLabels the ticksLabels to set
     */
    public UpdateStatus setTicksLabelsAsArrayList(ArrayList<String> ticksLabels) {
        if (!this.ticksLabels.isEmpty()) {
            this.ticksLabels.clear();
        }

        for (int i = 0; i < ticksLabels.size(); i++) {
            this.ticksLabels.add(new String(ticksLabels.get(i)));
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the ticksSegment
     */
    public Boolean getTicsSegment() {
        return ticksSegment;
    }

    /**
     * @param ticksSegment the ticksSegment to set
     */
    public UpdateStatus setTicsSegment(boolean ticksSegment) {
        this.ticksSegment = ticksSegment;
        return UpdateStatus.Success;
    }

    /**
     * @return the ticksStyle
     */
    public TicksStyle getTicsStyle() {
        return ticksStyle;
    }

    /**
     * @param ticksStyle the ticksStyle to set
     */
    public UpdateStatus setTicsStyle(TicksStyle ticksStyle) {
        this.ticksStyle = ticksStyle;
        return UpdateStatus.Success;
    }

    /**
     * @return the subticks
     */
    public Integer getSubtics() {
        return subticks;
    }

    /**
     * @param subticks the subticks to set
     */
    public UpdateStatus setSubtics(int subticks) {
        this.subticks = subticks;
        return UpdateStatus.Success;
    }

    /**
     * @return the number of X ticks
     */
    public Integer getXNumberTics() {
        return xTicksCoords.length;
    }

    /**
     * @return the number of X ticks
     */
    public Integer getYNumberTics() {
        return yTicksCoords.length;
    }

    /**
     * @return the xTicksCoords
     */
    public Double[] getXTicsCoord() {
        Double[] retXTicksCoords = new Double[xTicksCoords.length];

        for (int i = 0; i < xTicksCoords.length; i++) {
            retXTicksCoords[i] = xTicksCoords[i];
        }

        return retXTicksCoords;
    }

    /**
     * Sets the x ticks coordinates
     * Resizes the array if required
     * TODO : we should use format_n to fill ticks label.
     * @param ticksCoords the xTicksCoords to set
     */
    public UpdateStatus setXTicsCoord(double[] ticksCoords) {
        if (ticksCoords.length != xTicksCoords.length) {
            xTicksCoords = new double[ticksCoords.length];
        }

        for (int i = 0; i < xTicksCoords.length; i++) {
            xTicksCoords[i] = ticksCoords[i];
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the yTicksCoords
     */
    public Double[] getYTicsCoord() {
        Double[] retYTicksCoords = new Double[yTicksCoords.length];

        for (int i = 0; i < yTicksCoords.length; i++) {
            retYTicksCoords[i] = yTicksCoords[i];
        }

        return retYTicksCoords;
    }

    /**
     * Sets the y ticks coordinates
     * Resizes the array if required
     * TODO : we should use format_n to fill ticks label.
     * @param ticksCoords the yTicksCoords to set
     */
    public UpdateStatus setYTicsCoord(double[] ticksCoords) {
        if (ticksCoords.length != yTicksCoords.length) {
            yTicksCoords = new double[ticksCoords.length];
        }

        for (int i = 0; i < yTicksCoords.length; i++) {
            yTicksCoords[i] = ticksCoords[i];
        }
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_AXIS__;
    }

}
