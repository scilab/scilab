/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
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

package org.scilab.modules.graphic_objects.contouredObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Mark class
 * @author Manuel JULIACHS
 */
public class Mark {
    /** Mark properties */
    public enum MarkPropertyType { MODE, STYLE, SIZEUNIT, SIZE, FOREGROUND, BACKGROUND, FOREGROUNDALPHA, BACKGROUNDALPHA };

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

    /** Background (face) transparency (0.0 = fully transparent, 1.0 = fully opaque) */
    private double backgroundAlpha;

    /** Foreground (edge) transparency (0.0 = fully transparent, 1.0 = fully opaque) */
    private double foregroundAlpha;

    /** Constructor */
    public Mark() {
        super();
        mode = false;
        style = 0;
        markSizeUnit = MarkSizeUnitType.POINT;
        size = 0;
        foreground = 0;
        background = 0;
        foregroundAlpha = 1.0;
        backgroundAlpha = 1.0;
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
        foregroundAlpha = mark.foregroundAlpha;
        backgroundAlpha = mark.backgroundAlpha;
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
     * @return the background (face) transparency
     */
    public Double getBackgroundAlpha() {
        return backgroundAlpha;
    }

    /**
     * @param backgroundAlpha the background (face) transparency to set
     */
    public UpdateStatus setBackgroundAlpha(Double backgroundAlpha) {
        if (this.backgroundAlpha == backgroundAlpha) {
            return UpdateStatus.NoChange;
        }
        this.backgroundAlpha = backgroundAlpha;
        return UpdateStatus.Success;
    }

    /**
     * @return the foreground (edge) transparency
     */
    public Double getForegroundAlpha() {
        return foregroundAlpha;
    }

    /**
     * @param foregroundAlpha the foreground (edge) transparency to set
     */
    public UpdateStatus setForegroundAlpha(Double foregroundAlpha) {
        if (this.foregroundAlpha == foregroundAlpha) {
            return UpdateStatus.NoChange;
        }
        this.foregroundAlpha = foregroundAlpha;
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
