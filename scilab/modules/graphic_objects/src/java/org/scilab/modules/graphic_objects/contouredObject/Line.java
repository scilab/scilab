/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.utils.LineType;

/**
 * Line class
 * @author Manuel JULIACHS
 */
public class Line {
    /** Line properties */
    public enum LinePropertyType { MODE, LINESTYLE, THICKNESS, COLOR };

    /** Specifies whether the line is drawn or not */
    private boolean mode;

    /** Line style */
    private LineType lineStyle;

    /** Line thickness */
    private double thickness;

    /** Line color */
    private int color;

    /** Constructor */
    public Line() {
        mode = false;
        lineStyle = LineType.SOLID;
        thickness = 1.0;
        color = -1;
    }

    /**
     * Copy constructor
     * @param line the Line to copy
     */
    public Line(Line line) {
        mode = line.mode;
        lineStyle = line.lineStyle;
        thickness = line.thickness;
        color = line.color;
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
        if (this.color == color) {
            return UpdateStatus.NoChange;
        }
        this.color = color;
        return UpdateStatus.Success;
    }

    /**
     * @return the lineStyle
     */
    public LineType getLineStyle() {
        return lineStyle;
    }

    /**
     * @param lineStyle the lineStyle to set
     */
    public UpdateStatus setLineStyle(LineType lineStyle) {
        if (this.lineStyle != lineStyle) {
            this.lineStyle = lineStyle;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the mode
     */
    public Boolean getMode() {
        return mode;
    }

    /**
     * @param mode the mode to set
     */
    public UpdateStatus setMode(boolean mode) {
        if (this.mode == mode) {
            return UpdateStatus.NoChange;
        }
        this.mode = mode;
        return UpdateStatus.Success;
    }

    /**
     * @return the thickness
     */
    public Double getThickness() {
        return thickness;
    }

    /**
     * @param thickness the thickness to set
     */
    public UpdateStatus setThickness(double thickness) {
        if (this.thickness != thickness) {
            this.thickness = thickness;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }
}
