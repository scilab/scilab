/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
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

package org.scilab.modules.graphic_objects.figure;

import java.util.Arrays;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * @author Pierre Lando
 */
public class ColorMap {
    /**
     * Number of channels for one color.
     */
    private static final int CHANNELS_NUMBER = 3;

    private static final float[] BLACK_COLOR = new float[] {0, 0, 0};
    private static final float[] WHITE_COLOR = new float[] {1, 1, 1};
    private static final float[] LIGHT_GRAY_COLOR = new float[] {0.7f, 0.7f, 0.7f};

    /**
     * The data of this ColorMap.
     */
    Double[] data = new Double[0];

    /**
     * Default constructor
     */
    public ColorMap() {

    }

    /**
     * Copy constructor
     * @param colorMap the ColorMap to copy
     */
    public ColorMap(ColorMap colorMap) {
        setData(colorMap.data);
    }

    /**
     * Return a copy of the data of this color map.
     * @return a copy of the data of this color map.
     */
    public Double[] getData() {
        return data.clone();
    }

    /**
     * Return the number of color in this ColorMap
     * @return the number of color in this ColorMap
     */
    public int getSize() {
        return data.length / CHANNELS_NUMBER;
    }

    /**
     * Return the color corresponding to the given Scilab index.
     *
     * The first color in scilab ColorMap has index 1.
     * If index is 0, it will be considered as 1.
     * If index is -1 the color is black.
     * If index is -2 the color is white.
     * If index is last scilab index + 1, it will be considered as -1.
     * If index is last scilab index + 2, it will be considered as -2.
     * Other values should not be possible.
     * By the way we consider the nearest value.
     *
     * @param index the given Scilab index.
     * @return the color corresponding to the given Scilab index.
     */
    public float[] getScilabColor(int index) {

        int lastScilabIndex = getSize();

        if (index == 0) {
            index = -1;
        }

        if (index > lastScilabIndex) {
            index = lastScilabIndex - index;
        }
        if (index == -3) {
            return LIGHT_GRAY_COLOR;
        } else if (index == -1 || index < -2) {
            return BLACK_COLOR;
        } else if (index == -2) {
            return WHITE_COLOR;
        } else {
            index = index - 1;
            return new float[] {
                       Math.min(1.f, data[index].floatValue()),
                       Math.min(1.f, data[index + getSize()].floatValue()),
                       Math.min(1.f, data[index + getSize() * 2].floatValue())
                   };
        }
    }

    /**
     * Set the data of this ColorMap
     * This function is package to avoid bad use (update model without using the controller).
     * @param data the new data.
     */
    UpdateStatus setData(Double[] data) {
        // Do not update colormap if it's the same
        if (Arrays.equals(data, this.data)) {
            return UpdateStatus.NoChange;
        }
        int length = data.length;
        length -= length % CHANNELS_NUMBER;

        this.data = new Double[length];
        System.arraycopy(data, 0, this.data, 0, length);
        return UpdateStatus.Success;
    }
}
