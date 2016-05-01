/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.utils;

/**
 * Class Utils
 * A set of static methods used to determine the validity of points or values.
 * @author Manuel JULIACHS
 */
public class Utils {

    /**
     * Tests whether a scalar value is valid.
     * The value is valid if it is neither infinite nor a NaN.
     * @param the scalar value to test.
     * @return true if the value is valid, false if it is not.
     */
    public static boolean isValid(double x) {
        if (Double.isNaN(x) || Double.isInfinite(x)) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * Tests whether a point is valid.
     * The point is valid if none of its coordinates is infinite or a NaN.
     * @param the point's x-coordinate.
     * @param the point's y-coordinate.
     * @param the point's z-coordinate.
     * @return true if the point is valid, false if it is not.
     */
    public static boolean isValid(double x, double y, double z) {
        if (Double.isNaN(x) || Double.isNaN(y) || Double.isNaN(z) || Double.isInfinite(x) || Double.isInfinite(y) || Double.isInfinite(z)) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * Determines if a single-component point is valid in logarithmic scale.
     * @param x the point's coordinate.
     * @return true if the point is valid, false if it is not.
     */
    public static boolean isLogValid(double x) {
        if (x > 0.0) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Determines if a point is valid in logarithmic scale.
     * It checks whether any of its coordinates is strictly less than 0 (if the
     * logarithmic scale applies). In the event of the former, the point is
     * determined as not valid.
     * @param the point's x-coordinate.
     * @param the point's y-coordinate.
     * @param the point's z-coordinate.
     * @param the bit mask specifying for which coordinates the logarithmic scale is used.
     * @return true if the point is valid, false if it is not.
     */
    public static boolean isLogValid(double x, double y, double z, int logMask) {
        boolean valid = true;

        if ((logMask & 0x1) != 0) {
            valid = valid && (x > 0.0);
        }

        if ((logMask & 0x2) != 0) {
            valid = valid && (y > 0.0);
        }

        if ((logMask & 0x4) != 0) {
            valid = valid && (z > 0.0);
        }

        return valid;
    }
}
