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

package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * ClippableProperty class
 * @author Manuel JULIACHS
 */
public class ClippableProperty {
    /** ClippableProperty properties */
    public enum ClippablePropertyType { CLIPSTATE, CLIPBOX, CLIPBOXSET };

    /** Indicates how clipping is performed */
    public static enum ClipStateType { OFF, CLIPGRF, ON;

                                       /**
                                        * Converts an integer to the corresponding enum
                                        * @param intValue the integer value
                                        * @return the clip state type enum
                                        */
    public static ClipStateType intToEnum(Integer intValue) {
        switch (intValue) {
            case 0:
                return ClipStateType.OFF;
            case 1:
                return ClipStateType.CLIPGRF;
            case 2:
                return ClipStateType.ON;
            default:
                return null;
        }
    }

                                     }

    /** Clipping state */
    private ClipStateType clipState;

    /** Clip box (4- or 6-element array) */
    private double[] clipBox;

    /** Specifies whether the clip box has been set at least once */
    private boolean clipBoxSet;

    /** Constructor */
    public ClippableProperty() {
        clipState = ClipStateType.OFF;
        clipBox = new double[4];
        clipBoxSet = false;
    }

    /**
     * Copy constructor
     * @param clippableProperty the ClippableProperty to copy
     */
    public ClippableProperty(ClippableProperty clippableProperty) {
        clipState = clippableProperty.clipState;

        clipBox = new double[4];

        for (int i = 0; i < clipBox.length; i++) {
            clipBox[i] = clippableProperty.clipBox[i];
        }

        clipBoxSet = clippableProperty.clipBoxSet;
    }

    /**
     * @return the clipBox
     */
    public Double[] getClipBox() {
        Double[] retClipBox = new Double[clipBox.length];

        for (int i = 0; i < clipBox.length; i++) {
            retClipBox[i] = clipBox[i];
        }

        return retClipBox;
    }

    /**
     * @param clipBox the clipBox to set
     */
    public UpdateStatus setClipBox(Double[] clipBox) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (clipBoxSet == false) {
            clipBoxSet = true;
            status = UpdateStatus.Success;
        }

        if (status == UpdateStatus.Success) {
            for (int i = 0; i < clipBox.length; i++) {
                this.clipBox[i] = clipBox[i];
            }
        } else {
            for (int i = 0; i < clipBox.length; i++) {
                if (this.clipBox[i] != clipBox[i]) {
                    this.clipBox[i] = clipBox[i];
                    status = UpdateStatus.Success;
                }
            }
        }

        return status;
    }

    /**
     * @return the clipState
     */
    public ClipStateType getClipState() {
        return clipState;
    }

    /**
     * @param clipState the clipState to set
     */
    public UpdateStatus setClipState(ClipStateType clipState) {
        if (clipState == ClipStateType.ON && clipBoxSet == false) {
            clipState = ClipStateType.CLIPGRF;
        }

        if (this.clipState != clipState) {
            this.clipState = clipState;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the clipBoxSet
     */
    public Boolean getClipBoxSet() {
        return clipBoxSet;
    }

    /**
     * @param clipBoxSet the clipBoxSet to set
     */
    public UpdateStatus setClipBoxSet(Boolean clipBoxSet) {
        if (this.clipBoxSet != clipBoxSet) {
            this.clipBoxSet = clipBoxSet;
            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }
}
