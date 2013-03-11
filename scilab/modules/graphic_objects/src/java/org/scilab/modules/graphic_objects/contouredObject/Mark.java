/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Mark class
 * @author Manuel JULIACHS
 */
public class Mark {
    /** Mark properties */
    public enum MarkPropertyType { MODE, STYLE, SIZEUNIT, SIZE, FOREGROUND, BACKGROUND };

    /** Mark size unit type */
    public enum MarkSizeUnitType { POINT, TABULATED;

                                   /**
                                    * Converts an integer to the corresponding enum
                                    * @param intValue the integer value
                                    * @return the mark size unit type enum
                                    */
    public static MarkSizeUnitType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return MarkSizeUnitType.POINT;
            case 1:
                return MarkSizeUnitType.TABULATED;
            default:
                return null;
        }
    }
                                 }


    /** Specifies whether marks must be drawn or not */
    private boolean mode;

    /** Mark style */
    private int style;

    /** Mark size unit */
    private MarkSizeUnitType markSizeUnit;

    /** Mark size */
    private int size;

    /** Foreground color */
    private int foreground;

    /** Background color */
    private int background;

    /** Constructor */
    public Mark() {
        super();
        mode = false;
        style = 0;
        markSizeUnit = MarkSizeUnitType.POINT;
        foreground = 0;
        background = 0;
    }

    /**
     * Copy constructor
     * @param mark the Mark to copy
     */
    public Mark(Mark mark) {
        mode = mark.mode;
        style = mark.style;
        size = mark.size;
        markSizeUnit = mark.markSizeUnit;
        foreground = mark.foreground;
        background = mark.background;
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
        if (this.background != background) {
            this.background = background;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the foreground
     */
    public Integer getForeground() {
        return foreground;
    }

    /**
     * @param foreground the foreground to set
     */
    public UpdateStatus setForeground(Integer foreground) {
        if (this.foreground != foreground) {
            this.foreground = foreground;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the markSizeUnit
     */
    public MarkSizeUnitType getMarkSizeUnit() {
        return markSizeUnit;
    }

    /**
     * @param markSizeUnit the markSizeUnit to set
     */
    public UpdateStatus setMarkSizeUnit(MarkSizeUnitType markSizeUnit) {
        if (this.markSizeUnit != markSizeUnit) {
            this.markSizeUnit = markSizeUnit;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the size
     */
    public Integer getSize() {
        return size;
    }

    /**
     * @param size the size to set
     */
    public UpdateStatus setSize(Integer size) {
        if (this.size == size) {
            return UpdateStatus.NoChange;
        }
        this.size = size;
        return UpdateStatus.Success;
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
    public UpdateStatus setMode(Boolean mode) {
        if (this.mode == mode) {
            return UpdateStatus.NoChange;
        }
        this.mode = mode;
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
        if (this.style == style) {
            return UpdateStatus.NoChange;
        }
        this.style = style;
        return UpdateStatus.Success;
    }
}
