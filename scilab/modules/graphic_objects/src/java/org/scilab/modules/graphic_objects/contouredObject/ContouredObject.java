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

package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.contouredObject.Line.LinePropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Line.LineType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkPropertyType;
import org.scilab.modules.graphic_objects.contouredObject.Mark.MarkSizeUnitType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Contoured object class
 * @author Manuel JULIACHS
 */
public abstract class ContouredObject extends GraphicObject {
    /** ContouredObject properties */
    public enum ContouredObjectPropertyType { LINE, FILLMODE, BACKGROUND, MARK, MARK_OFFSET, MARK_STRIDE, SELECTED };

    /** Line property */
    private Line line;

    /** Specifies whether the object is filled or not */
    private boolean fillMode;

    /** Background (fill) color */
    private int background;

    /** Mark property */
    private Mark mark;

    private int offset;
    private int stride;

    private boolean selected;
    private final Integer selectedColor = new Integer(-3);

    /** Default constructor */
    public ContouredObject() {
        line = new Line();
        fillMode = false;
        background = 0;
        mark = new Mark();
        offset = 0;
        stride = 1;
        selected = false;
    }

    public ContouredObject clone() {
        ContouredObject copy = (ContouredObject) super.clone();

        copy.line = new Line(line);
        copy.mark = new Mark(mark);

        return copy;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_LINE__ :
                return ContouredObjectPropertyType.LINE;
            case __GO_LINE_MODE__ :
                return LinePropertyType.MODE;
            case __GO_LINE_STYLE__ :
                return LinePropertyType.LINESTYLE;
            case __GO_LINE_THICKNESS__ :
                return LinePropertyType.THICKNESS;
            case __GO_LINE_COLOR__ :
                return LinePropertyType.COLOR;
            case __GO_FILL_MODE__ :
                return ContouredObjectPropertyType.FILLMODE;
            case __GO_BACKGROUND__ :
                return ContouredObjectPropertyType.BACKGROUND;
            case __GO_MARK__ :
                return ContouredObjectPropertyType.MARK;
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
            case __GO_MARK_OFFSET__ :
                return ContouredObjectPropertyType.MARK_OFFSET;
            case __GO_MARK_STRIDE__ :
                return ContouredObjectPropertyType.MARK_STRIDE;
            case __GO_SELECTED__ :
                return ContouredObjectPropertyType.SELECTED;
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
        if (property == ContouredObjectPropertyType.LINE) {
            return getLine();
        } else if (property == LinePropertyType.MODE) {
            return getLineMode();
        } else if (property == LinePropertyType.LINESTYLE) {
            return getLineStyle();
        } else if (property == LinePropertyType.THICKNESS) {
            return getLineThickness();
        } else if (property == LinePropertyType.COLOR) {
            return getLineColor();
        } else if (property == ContouredObjectPropertyType.FILLMODE) {
            return getFillMode();
        } else if (property == ContouredObjectPropertyType.BACKGROUND) {
            return getBackground();
        } else if (property == ContouredObjectPropertyType.MARK) {
            return getMark();
        } else if (property == MarkPropertyType.MODE) {
            return getMarkMode();
        } else if (property == MarkPropertyType.STYLE) {
            return getMarkStyle();
        } else if (property == MarkPropertyType.SIZEUNIT) {
            return getMarkSizeUnit();
        } else if (property == MarkPropertyType.SIZE) {
            return getMarkSize();
        } else if (property == MarkPropertyType.FOREGROUND) {
            return getMarkForeground();
        } else if (property == MarkPropertyType.BACKGROUND) {
            return getMarkBackground();
        } else if (property == ContouredObjectPropertyType.MARK_OFFSET) {
            return getMarkOffset();
        } else if (property == ContouredObjectPropertyType.MARK_STRIDE) {
            return getMarkStride();
        } else if (property == ContouredObjectPropertyType.SELECTED) {
            return getSelected();
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
        if (property == ContouredObjectPropertyType.LINE) {
            setLine((Line) value);
        } else if (property == LinePropertyType.MODE) {
            return setLineMode((Boolean) value);
        } else if (property == LinePropertyType.LINESTYLE) {
            setLineStyle((Integer) value);
        } else if (property == LinePropertyType.THICKNESS) {
            setLineThickness((Double) value);
        } else if (property == LinePropertyType.COLOR) {
            return setLineColor((Integer) value);
        } else if (property == ContouredObjectPropertyType.FILLMODE) {
            return setFillMode((Boolean) value);
        } else if (property == ContouredObjectPropertyType.BACKGROUND) {
            setBackground((Integer) value);
        } else if (property == ContouredObjectPropertyType.MARK) {
            setMark((Mark) value);
        } else if (property == MarkPropertyType.MODE) {
            return setMarkMode((Boolean) value);
        } else if (property == MarkPropertyType.STYLE) {
            return setMarkStyle((Integer) value);
        } else if (property == MarkPropertyType.SIZEUNIT) {
            setMarkSizeUnit((Integer) value);
        } else if (property == MarkPropertyType.SIZE) {
            return setMarkSize((Integer) value);
        } else if (property == MarkPropertyType.FOREGROUND) {
            this.setMarkForeground((Integer) value);
        } else if (property == MarkPropertyType.BACKGROUND) {
            this.setMarkBackground((Integer) value);
        } else if (property == ContouredObjectPropertyType.MARK_OFFSET) {
            this.setMarkOffset((Integer) value);
        } else if (property == ContouredObjectPropertyType.MARK_STRIDE) {
            this.setMarkStride((Integer) value);
        } else if (property == ContouredObjectPropertyType.SELECTED) {
            this.setSelected((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the color to use for selection
     */
    public Integer getSelectedColor() {
        return selectedColor;
    }

    /**
     * @return true if selected
     */
    public Boolean getSelected() {
        return selected;
    }

    /**
     * @param selected true if selected
     */
    public UpdateStatus setSelected(Boolean selected) {
        if (this.selected != selected) {
            this.selected = selected;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the offset
     */
    public Integer getMarkOffset() {
        return offset;
    }

    /**
     * @param offset the offset to set
     */
    public UpdateStatus setMarkOffset(Integer offset) {
        if (this.offset != offset) {
            this.offset = offset < 0 ? 0 : offset;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the stride
     */
    public Integer getMarkStride() {
        return stride;
    }

    /**
     * @param stride the stride to set
     */
    public UpdateStatus setMarkStride(Integer stride) {
        if (this.stride != stride) {
            this.stride = stride < 1 ? 1 : stride;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
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
        this.background = background;
        return UpdateStatus.Success;
    }

    /**
     * @return the fillMode
     */
    public Boolean getFillMode() {
        return fillMode;
    }

    /**
     * @param fillMode the fillMode to set
     */
    public UpdateStatus setFillMode(Boolean fillMode) {
        if (this.fillMode == fillMode) {
            return UpdateStatus.NoChange;
        }
        this.fillMode = fillMode;
        return UpdateStatus.Success;
    }

    /**
     * @return the line
     */
    public Line getLine() {
        return line;
    }

    /**
     * @param line the line to set
     */
    public UpdateStatus setLine(Line line) {
        this.line = line;
        return UpdateStatus.Success;
    }

    /**
     * Get the line color
     * @return the color
     */
    public Integer getLineColor() {
        return line.getColor();
    }

    /**
     * Set the line color
     * @param color the color to set
     */
    public UpdateStatus setLineColor(Integer color) {
        return line.setColor(color);
    }

    /**
     * Get the line style
     * @return the lineStyle
     */
    public Integer getLineStyle() {
        return getLineStyleAsEnum().asScilabIndex();
    }

    /**
     * Get the line style
     * @return the lineStyle
     */
    public LineType getLineStyleAsEnum() {
        return line.getLineStyle();
    }

    /**
     * Set the line style
     * @param lineStyle the lineStyle to set
     */
    public UpdateStatus setLineStyle(Integer lineStyle) {
        setLineStyleAsEnum(LineType.fromScilabIndex(lineStyle));
        return UpdateStatus.Success;
    }

    /**
     * Set the line style
     * @param lineStyle the lineStyle to set
     */
    public UpdateStatus setLineStyleAsEnum(LineType lineStyle) {
        line.setLineStyle(lineStyle);
        return UpdateStatus.Success;
    }

    /**
     * Return the line mode
     * @return the mode
     */
    public Boolean getLineMode() {
        return line.getMode();
    }

    /**
     * Set the line mode
     * @param mode the mode to set
     */
    public UpdateStatus setLineMode(Boolean mode) {
        return line.setMode(mode);
    }

    /**
     * Get the line thickness
     * @return the thickness
     */
    public Double getLineThickness() {
        return line.getThickness();
    }

    /**
     * Set the line thickness
     * @param thickness the thickness to set
     */
    public UpdateStatus setLineThickness(Double thickness) {
        line.setThickness(thickness);
        return UpdateStatus.Success;
    }

    /**
     * @return the mark
     */
    public Mark getMark() {
        return mark;
    }

    /**
     * @param mark the mark to set
     */
    public UpdateStatus setMark(Mark mark) {
        this.mark = mark;
        return UpdateStatus.Success;
    }

    /**
     * Get the mark background
     * @return the background
     */
    public Integer getMarkBackground() {
        return mark.getBackground();
    }

    /**
     * Set the mark background
     * @param background the background to set
     */
    public UpdateStatus setMarkBackground(Integer background) {
        mark.setBackground(background);
        return UpdateStatus.Success;
    }

    /**
     * Get the mark foreground
     * @return the foreground
     */
    public Integer getMarkForeground() {
        return mark.getForeground();
    }

    /**
     * Set the mark foreground
     * @param foreground the foreground to set
     */
    public UpdateStatus setMarkForeground(Integer foreground) {
        mark.setForeground(foreground);
        return UpdateStatus.Success;
    }

    /**
     * Get the mark size unit
     * @return the markSizeUnit
     */
    public Integer getMarkSizeUnit() {
        return getMarkSizeUnitAsEnum().ordinal();
    }

    /**
     * Get the mark size unit
     * @return the markSizeUnit
     */
    public MarkSizeUnitType getMarkSizeUnitAsEnum() {
        return mark.getMarkSizeUnit();
    }

    /**
     * Set the mark size unit
     * @param markSizeUnit the markSizeUnit to set
     */
    public UpdateStatus setMarkSizeUnit(Integer markSizeUnit) {
        setMarkSizeUnitAsEnum(MarkSizeUnitType.intToEnum(markSizeUnit));
        return UpdateStatus.Success;
    }

    /**
     * Set the mark size unit
     * @param markSizeUnit the markSizeUnit to set
     */
    public UpdateStatus setMarkSizeUnitAsEnum(MarkSizeUnitType markSizeUnit) {
        mark.setMarkSizeUnit(markSizeUnit);
        return UpdateStatus.Success;
    }

    /**
     * Get the mark size
     * @return the size
     */
    public Integer getMarkSize() {
        return mark.getSize();
    }

    /**
     * Set the mark size
     * @param size the size to set
     */
    public UpdateStatus setMarkSize(Integer size) {
        return mark.setSize(size);
    }

    /**
     * Get the mark mode
     * @return the mode
     */
    public Boolean getMarkMode() {
        return mark.getMode();
    }

    /**
     * Set the mark mode
     * @param mode the mode to set
     */
    public UpdateStatus setMarkMode(Boolean mode) {
        return mark.setMode(mode);
    }

    /**
     * Get the mark style
     * @return the style
     */
    public Integer getMarkStyle() {
        return mark.getStyle();
    }

    /**
     * Set the mark style
     * @param style the style to set
     */
    public UpdateStatus setMarkStyle(Integer style) {
        return mark.setStyle(style);
    }

}
