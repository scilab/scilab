/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.ruler;

import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.text.DecimalFormat;
import java.util.List;

/**
 * Contain  information about the ruler drawing result.
 *
 * @author Pierre Lando
 */
public class RulerDrawingResult {

    /**
     * Values of drawn ticks.
     */
    private final double[] ticksValues;

    /**
     * Values of drawn sub-ticks.
     */
    private final double[] subTicksValues;

    /**
     * Sub-ticks density.
     */
    private final int subTicksDensity;

    /**
     * The ratio between the maximum distance to any of the sprites' farthest sides
     * along the projected ticks direction and the projected ticks direction norm.
     */
    private final double maxDistToTicksDirNorm;

    /**
     * The normalized projected ticks direction.
     */
    private final Vector3d normalizedTicksDirection;

    /** The used format to draw sprites */
    private final DecimalFormat format;

    /**
     * Package constructor.
     * Those object can only be created by a {@link RulerDrawer}
     *
     * @param format
     * @param ticksValues values of drawn ticks.
     * @param subTicksValues values of drawn sub-ticks.
     * @param subTicksDensity sub-ticks density.
     * @param maxDistToTicksDirNorm the ratio between the maximum sprite distance and the projected ticks direction norm.
     * @param ticksDir the normalized projected ticks direction.
     */
    RulerDrawingResult(DecimalFormat format, List<Double> ticksValues, List<Double> subTicksValues, int subTicksDensity, double maxDistToTicksDirNorm, Vector3d ticksDir) {
        this.format = format;
        this.ticksValues = listToArray(ticksValues);
        this.subTicksValues = listToArray(subTicksValues);
        this.subTicksDensity = subTicksDensity;
        this.maxDistToTicksDirNorm = maxDistToTicksDirNorm;
        this.normalizedTicksDirection = new Vector3d(ticksDir);
    }

    /**
     * Format getter
     * @return the format.
     */
    public DecimalFormat getFormat() {
        return format;
    }

    /**
     * Convert a list of Double to an array of double.
     * @param list the given list.
     * @return an array of double with the value of the given list.
     */
    private double[] listToArray(List<Double> list) {
        double[] array = new double[list.size()];
        int index = 0;
        for (double value : list) {
            array[index++] = value;
        }
        return array;
    }

    /**
     * Drawn ticks values getter.
     * @return the drawn ticks values.
     */
    public double[] getTicksValues() {
        return ticksValues.clone();
    }

    /**
     * Drawn sub-ticks values getter.
     * @return the drawn sub-ticks values.
     */
    public double[] getSubTicksValues() {
        return subTicksValues.clone();
    }

    /**
     * Drawn sub-ticks density getter.
     * @return the drawn sub-ticks density.
     */
    public int getSubTicksDensity() {
        return subTicksDensity;
    }

    /**
     * Maximum sprite distance to projected ticks direction norm ratio getter.
     * @return the distance to ticks direction norm ratio.
     */
    public double getMaxDistToTicksDirNorm() {
        return maxDistToTicksDirNorm;
    }

    /**
     * Normalized projected ticks direction getter.
     * @return the normalized projected ticks direction.
     */
    public Vector3d getNormalizedTicksDirection() {
        return normalizedTicksDirection;
    }
}
