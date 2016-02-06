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

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Box class
 * @author Manuel JULIACHS
 */
public class Box {
    /** Box properties names */
    public enum BoxProperty { BOX, HIDDENAXISCOLOR, XTIGHTLIMITS, YTIGHTLIMITS, ZTIGHTLIMITS, DATABOUNDS, REALDATABOUNDS, ZOOMENABLED, ZOOMBOX, AUTOSCALE, FIRSTPLOT };

    /** Box type */
    public static enum BoxType { OFF, ON, HIDDEN_AXES, BACK_HALF;

                                 /**
                                  * Converts an integer to the corresponding enum
                                  * @param intValue the integer value
                                  * @return the box type enum
                                  */
    public static BoxType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return BoxType.OFF;
            case 1:
                return BoxType.ON;
            case 2:
                return BoxType.HIDDEN_AXES;
            case 3:
                return BoxType.BACK_HALF;
            default:
                return null;
        }
    }
                               }

    /** Box type */
    private BoxType box;

    /** Hidden axis color */
    private int hiddenAxisColor;

    /** Specifies whether tight limits are enforced or not */
    private boolean[] tightLimits;

    /** Data bounding box (6-element array) */
    private double[] dataBounds;

    /** Data bounding box, as modified by automatic ticks computation (6-element array) */
    private double[] realDataBounds;

    /** Speficies whether zooming is enabled or not */
    private boolean zoomEnabled;

    /** Magnified 3D sub-region (6-element array) */
    private double[] zoomBox;

    /** Indicates whether data bounds are updated when a new plot command is executed */
    private boolean autoScale;

    /**
     * Indicates whether no high-level drawing function has yet been called (true) or
     * has been called at least once (false)
     */
    private boolean firstPlot;

    /** Constructor */
    public Box() {
        box = BoxType.OFF;
        hiddenAxisColor = 0;
        tightLimits = new boolean[] {false, false, false};
        dataBounds = new double[] {0, 1, 0, 1, -1, 1};
        realDataBounds = new double[6];
        zoomEnabled = false;
        zoomBox = new double[6];
        autoScale = false;
        firstPlot = true;
    }

    /**
     * Copy constructor
     * @param box the Box to copy
     */
    public Box(Box box) {
        this.box = box.box;
        hiddenAxisColor = box.hiddenAxisColor;

        tightLimits = new boolean[3];
        for (int i = 0; i < tightLimits.length; i++) {
            tightLimits[i] = box.tightLimits[i];
        }

        dataBounds = new double[6];

        for (int i = 0; i < dataBounds.length; i++) {
            dataBounds[i] = box.dataBounds[i];
        }

        realDataBounds = new double[6];

        for (int i = 0; i < realDataBounds.length; i++) {
            realDataBounds[i] = box.realDataBounds[i];
        }

        zoomEnabled = box.zoomEnabled;

        zoomBox = new double[6];

        for (int i = 0; i < zoomBox.length; i++) {
            zoomBox[i] = box.zoomBox[i];
        }

        autoScale = box.autoScale;
        firstPlot = box.firstPlot;
    }

    /**
     * @return the autoScale
     */
    public Boolean getAutoScale() {
        return autoScale;
    }

    /**
     * @param autoScale the autoScale to set
     */
    public UpdateStatus setAutoScale(Boolean autoScale) {
        if (this.autoScale != autoScale) {
            this.autoScale = autoScale;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the box
     */
    public BoxType getBox() {
        return box;
    }

    /**
     * @param box the box to set
     */
    public UpdateStatus setBox(BoxType box) {
        if (this.box != box) {
            this.box = box;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * Return the data bounds.
     * The array contain : {xMin, xMax, yMin, yMax, zMin, zMax}
     * @return the dataBounds
     */
    public Double[] getDataBounds() {
        Double[] retDataBounds = new Double[6];

        for (int i = 0; i < retDataBounds.length; i++) {
            retDataBounds[i] = dataBounds[i];
        }

        return retDataBounds;
    }

    /**
     * @param dataBounds the dataBounds to set
     */
    public UpdateStatus setDataBounds(Double[] dataBounds) {
        UpdateStatus status = UpdateStatus.NoChange;
        final int len = Math.min(this.dataBounds.length, dataBounds.length);
        for (int i = 0; i < len; i++) {
            if (this.dataBounds[i] != dataBounds[i]) {
                this.dataBounds[i] = dataBounds[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    /**
     * @return the hiddenAxisColor
     */
    public Integer getHiddenAxisColor() {
        return hiddenAxisColor;
    }

    /**
     * @param hiddenAxisColor the hiddenAxisColor to set
     */
    public UpdateStatus setHiddenAxisColor(Integer hiddenAxisColor) {
        if (this.hiddenAxisColor != hiddenAxisColor) {
            this.hiddenAxisColor = hiddenAxisColor;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the realDataBounds
     */
    public Double[] getRealDataBounds() {
        Double[] retRealDataBounds = new Double[6];

        for (int i = 0; i < retRealDataBounds.length; i++) {
            retRealDataBounds[i] = realDataBounds[i];
        }

        return retRealDataBounds;
    }

    /**
     * @param realDataBounds the realDataBounds to set
     */
    public UpdateStatus setRealDataBounds(Double[] realDataBounds) {
        UpdateStatus status = UpdateStatus.NoChange;
        final int len = Math.min(this.realDataBounds.length, realDataBounds.length);
        for (int i = 0; i < len; i++) {
            if (this.realDataBounds[i] != realDataBounds[i]) {
                this.realDataBounds[i] = realDataBounds[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    /**
     * @return the tightLimits for X axis
     */
    public Boolean getXTightLimits() {
        return tightLimits[0];
    }

    /**
     * @return the tightLimits for Y axis
     */
    public Boolean getYTightLimits() {
        return tightLimits[1];
    }

    /**
     * @return the tightLimits for X axis
     */
    public Boolean getZTightLimits() {
        return tightLimits[2];
    }

    /**
     * @param tightLimits the tightLimits to set for X axis
     */
    public UpdateStatus setXTightLimits(Boolean tightLimits) {
        if (this.tightLimits[0] != tightLimits) {
            this.tightLimits[0] = tightLimits;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @param tightLimits the tightLimits to set for Y axis
     */
    public UpdateStatus setYTightLimits(Boolean tightLimits) {
        if (this.tightLimits[1] != tightLimits) {
            this.tightLimits[1] = tightLimits;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @param tightLimits the tightLimits to set for Z axis
     */
    public UpdateStatus setZTightLimits(Boolean tightLimits) {
        if (this.tightLimits[2] != tightLimits) {
            this.tightLimits[2] = tightLimits;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the zoomEnabled
     */
    public Boolean getZoomEnabled() {
        return zoomEnabled;
    }

    /**
     * @param zoomEnabled the zoomEnabled to set
     */
    public UpdateStatus setZoomEnabled(Boolean zoomEnabled) {
        if (this.zoomEnabled != zoomEnabled) {
            this.zoomEnabled = zoomEnabled;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the zoomBox
     */
    public Double[] getZoomBox() {
        Double[] retZoomBox = new Double[6];

        for (int i = 0; i < retZoomBox.length; i++) {
            retZoomBox[i] = zoomBox[i];
        }

        return retZoomBox;
    }

    /**
     * @param zoomBox the zoomBox to set
     */
    public UpdateStatus setZoomBox(Double[] zoomBox) {
        UpdateStatus status = UpdateStatus.NoChange;
        final int len = Math.min(this.zoomBox.length, zoomBox.length);
        for (int i = 0; i < len; i++) {
            if (this.zoomBox[i] != zoomBox[i]) {
                this.zoomBox[i] = zoomBox[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    /**
     * @return the firstPlot
     */
    public Boolean getFirstPlot() {
        return firstPlot;
    }

    /**
     * @param firstPlot the firstPlot to set
     */
    public UpdateStatus setFirstPlot(Boolean firstPlot) {
        if (this.firstPlot != firstPlot) {
            this.firstPlot = firstPlot;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }
}
