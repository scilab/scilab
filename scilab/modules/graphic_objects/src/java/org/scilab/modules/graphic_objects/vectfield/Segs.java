/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.vectfield;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_BACKGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_FOREGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_SIZE_UNIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARK_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SEGS_COLORS__;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.contouredObject.Mark;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Segs class
 * @author Manuel JULIACHS
 */
public class Segs extends VectField {
    /** Sets properties names */
    private enum SegsProperty { COLORS };

    /** Constructor */
    public Segs() {
        super();
        // TODO Auto-generated constructor stub
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_SEGS_COLORS__ :
                return SegsProperty.COLORS;
            case __GO_MARK_MODE__ :
                return Mark.MarkPropertyType.MODE;
            case __GO_MARK_STYLE__ :
                return Mark.MarkPropertyType.STYLE;
            case __GO_MARK_SIZE_UNIT__ :
                return Mark.MarkPropertyType.SIZEUNIT;
            case __GO_MARK_SIZE__ :
                return Mark.MarkPropertyType.SIZE;
            case __GO_MARK_FOREGROUND__ :
                return Mark.MarkPropertyType.FOREGROUND;
            case __GO_MARK_BACKGROUND__ :
                return Mark.MarkPropertyType.BACKGROUND;
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
        if (property == SegsProperty.COLORS) {
            return getColors();
        } else if (property == Mark.MarkPropertyType.MODE) {
            return getMarkMode();
        } else if (property == Mark.MarkPropertyType.STYLE) {
            return getMarkStyle();
        } else if (property == Mark.MarkPropertyType.SIZEUNIT) {
            return getMarkSizeUnit();
        } else if (property == Mark.MarkPropertyType.SIZE) {
            return getMarkSize();
        } else if (property == Mark.MarkPropertyType.FOREGROUND) {
            return getMarkForeground();
        } else if (property == Mark.MarkPropertyType.BACKGROUND) {
            return getMarkBackground();
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
        if (property == SegsProperty.COLORS) {
            setColors((Integer[]) value);
        } else if (property == Mark.MarkPropertyType.MODE) {
            setMarkMode((Boolean) value);
        } else if (property == Mark.MarkPropertyType.STYLE) {
            setMarkStyle((Integer) value);
        } else if (property == Mark.MarkPropertyType.SIZEUNIT) {
            setMarkSizeUnit((Integer) value);
        } else if (property == Mark.MarkPropertyType.SIZE) {
            setMarkSize((Integer) value);
        } else if (property == Mark.MarkPropertyType.FOREGROUND) {
            setMarkForeground((Integer) value);
        } else if (property == Mark.MarkPropertyType.BACKGROUND) {
            setMarkBackground((Integer) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
     * Sets the number of arrows and resizes the arrow array
     * The actual resizing of the arrow array is performed in
     * the parent class' method.
     * The color of each arrow is reset to 0 in case of a size
     * change for compatibility with the previous behaviour of Segs.
     * To be modified
     * @param number the number of arrows to set
     */
    public UpdateStatus setNumberArrows(Integer number) {
        int currentNumber;

        currentNumber = arrows.size();

        super.setNumberArrows(number);

        if (currentNumber != number) {
            for (int i = 0; i < arrows.size(); i++) {
                arrows.get(i).setLineColor(0);
            }
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the segs colors
     */
    public Integer[] getColors() {
        Integer[] retSegsColors = new Integer[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            retSegsColors[i] = arrows.get(i).getLineColor();
        }

        return retSegsColors;
    }

    /**
     * Sets the segment colors
     * If the input color array contains only one element, each arrow's color is set to the latter.
     * @param colors the segs colors to set (number of elements either 1 or equal to the number of arrows)
     */
    public UpdateStatus setColors(Integer[] colors) {
        if (colors.length == 1) {
            for (int i = 0; i < arrows.size(); i++) {
                arrows.get(i).setLineColor(colors[0]);
            }
        } else {
            for (int i = 0; i < arrows.size(); i++) {
                arrows.get(i).setLineColor(colors[i]);
            }
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark mode
     * To be done: return an array of Boolean
     * @return the mark mode
     */
    public Boolean getMarkMode() {
        return arrows.get(0).getMarkMode();
    }

    /**
     * All segments are supposed to have the same mark mode
     * To be done: change the argument to an array of Boolean
     * @param mode the mark mode to set
     */
    public UpdateStatus setMarkMode(Boolean mode) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkMode(mode);
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark style
     * To be done: return an array of Integer
     * @return the mark style
     */
    public Integer getMarkStyle() {
        return arrows.get(0).getMarkStyle();
    }

    /**
     * All segments are supposed to have the same mark style
     * To be done: change the argument to an array of Integer
     * @param style the mark style to set
     */
    public UpdateStatus setMarkStyle(Integer style) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkStyle(style);
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark size unit
     * To be done: return an array of Integer
     * @return the mark size unit
     */
    public Integer getMarkSizeUnit() {
        return arrows.get(0).getMarkSizeUnit();
    }

    /**
     * All segments are supposed to have the same mark size unit
     * To be done: change the argument to an array of Integer
     * @param style the mark size unit to set
     */
    public UpdateStatus setMarkSizeUnit(Integer sizeUnit) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkSizeUnit(sizeUnit);
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark size
     * To be done: return an array of Integer
     * @return the mark size
     */
    public Integer getMarkSize() {
        return arrows.get(0).getMarkSize();
    }

    /**
     * All segments are supposed to have the same mark size
     * To be done: change the argument to an array of Integer
     * @param style the mark size to set
     */
    public UpdateStatus setMarkSize(Integer size) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkSize(size);
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark foreground
     * To be done: return an array of Integer
     * @return the mark foreground
     */
    public Integer getMarkForeground() {
        return arrows.get(0).getMarkForeground();
    }

    /**
     * All segments are supposed to have the same mark foreground
     * To be done: change the argument to an array of Integer
     * @param style the mark foreground to set
     */
    public UpdateStatus setMarkForeground(Integer foreground) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkForeground(foreground);
        }
        return UpdateStatus.Success;
    }

    /**
     * All segments are supposed to have the same mark background
     * To be done: return an array of Integer
     * @return the mark background
     */
    public Integer getMarkBackground() {
        return arrows.get(0).getMarkBackground();
    }

    /**
     * All segments are supposed to have the same mark background
     * To be done: change the argument to an array of Integer
     * @param style the mark background to set
     */
    public UpdateStatus setMarkBackground(Integer background) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setMarkBackground(background);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_SEGS__;
    }

}

