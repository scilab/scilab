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

package org.scilab.modules.graphic_objects.textObject;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_FRACTIONAL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FONT_STYLE__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Font class
 * @author Manuel JULIACHS
 */
public class Font {
    /** Font properties names */
    public enum FontProperty { STYLE, SIZE, COLOR, FRACTIONAL, UNKNOWNPROPERTY };

    /** Font style */
    private int style;

    /** Font size */
    private double size;

    /** Font color */
    private int color;

    /** Specifies whether fractional font sizes are used or not */
    private boolean fractional;

    /** Constructor */
    public Font() {
        this.style = 6;
        this.size = 1.0;
        this.color = -1;
        this.fractional = false;
    }

    /**
     * Copy constructor
     * @param
     */
    public Font(Font font) {
        style = font.style;
        size = font.size;
        color = font.color;
        fractional = font.fractional;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_FONT_STYLE__)) {
            return FontProperty.STYLE;
        } else if (propertyName.equals(__GO_FONT_SIZE__)) {
            return FontProperty.SIZE;
        } else if (propertyName.equals(__GO_FONT_COLOR__)) {
            return FontProperty.COLOR;
        } else if (propertyName.equals(__GO_FONT_FRACTIONAL__)) {
            return FontProperty.FRACTIONAL;
        } else {
            return FontProperty.UNKNOWNPROPERTY;
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == FontProperty.STYLE) {
            return getStyle();
        } else if (property == FontProperty.SIZE) {
            return getSize();
        } else if (property == FontProperty.COLOR) {
            return getColor();
        } else if (property == FontProperty.FRACTIONAL) {
            return getFractional();
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
        if (property == FontProperty.STYLE) {
            setStyle((Integer) value);
        } else if (property == FontProperty.SIZE) {
            setSize((Double) value);
        } else if (property == FontProperty.COLOR) {
            setColor((Integer) value);
        } else if (property == FontProperty.FRACTIONAL) {
            setFractional((Boolean) value);
        }

        return true;
    }

    /**
     * @return the color
     */
    public Integer getColor() {
        return color;
    }

    /**
     * @param color the color to set
     */
    public UpdateStatus setColor(Integer color) {
        this.color = color;
        return UpdateStatus.Success;
    }

    /**
     * @return the fractional
     */
    public Boolean getFractional() {
        return fractional;
    }

    /**
     * @param fractional the fractional to set
     */
    public UpdateStatus setFractional(Boolean fractional) {
        this.fractional = fractional;
        return UpdateStatus.Success;
    }

    /**
     * @return the size
     */
    public Double getSize() {
        return size;
    }

    /**
     * @param size the size to set
     */
    public UpdateStatus setSize(Double size) {
        this.size = size;
        return UpdateStatus.Success;
    }

    /**
     * @return the style
     */
    public Integer getStyle() {
        return style;
    }

    /**
     * @param style the style to set
     */
    public UpdateStatus setStyle(Integer style) {
        this.style = style;
        return UpdateStatus.Success;
    }

}
