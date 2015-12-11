/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * Class ScaleUtils
 * A set of utility methods for logarithmic scaling and unscaling.
 *
 * @author Manuel JULIACHS
 */
public class ScaleUtils {

    /**
     * Applies logarithmic scaling to a point and returns the result.
     * @param point the point.
     * @param logFlags the flags indicating whether scaling must be applied for respectively the x,y,z axes (3 elements).
     * @return the transformed point.
     */
    public static Vector3d applyLogScale(Vector3d point, boolean[] logFlags) {
        double[] coords = point.getData();

        if (logFlags[0]) {
            coords[0] = Math.log10(coords[0]);
        }

        if (logFlags[1]) {
            coords[1] = Math.log10(coords[1]);
        }

        if (logFlags[2]) {
            coords[2] = Math.log10(coords[2]);
        }

        return new Vector3d(coords);
    }

    /**
     * Applies logarithmic scaling to a coords array.
     * @param coords the coords (a 3-array).
     * @param logFlags the flags indicating whether scaling must be applied for respectively the x,y,z axes (3 elements).
     */
    public static void applyLogScale(double[] coords, boolean[] logFlags) {
        if (logFlags[0]) {
            coords[0] = Math.log10(coords[0]);
        }

        if (logFlags[1]) {
            coords[1] = Math.log10(coords[1]);
        }

        if (logFlags[2]) {
            coords[2] = Math.log10(coords[2]);
        }
    }

    /**
     * Applies inverse logarithmic scaling to a point and returns the result.
     * @param point the point.
     * @param logFlags the flags indicating whether inverse scaling must be applied for respectively the x,y,z axes (3 elements).
     * @return the transformed point.
     */
    public static Vector3d applyInverseLogScale(Vector3d point, boolean[] logFlags) {
        double[] coords = point.getData();

        if (logFlags[0]) {
            coords[0] = Math.pow(10.0, coords[0]);
        }

        if (logFlags[1]) {
            coords[1] = Math.pow(10.0, coords[1]);
        }

        if (logFlags[2]) {
            coords[2] = Math.pow(10.0, coords[2]);
        }

        return new Vector3d(coords);
    }

    /**
     * Applies inverse logarithmic scaling to a coords array.
     * @param coords the coords (a 3-array).
     * @param logFlags the flags indicating whether inverse scaling must be applied for respectively the x,y,z axes (3 elements).
     */
    public static void applyInverseLogScale(double[] coords, boolean[] logFlags) {
        if (logFlags[0]) {
            coords[0] = Math.pow(10.0, coords[0]);
        }

        if (logFlags[1]) {
            coords[1] = Math.pow(10.0, coords[1]);
        }

        if (logFlags[2]) {
            coords[2] = Math.pow(10.0, coords[2]);
        }
    }

    /**
     * Applies inverse logarithmic scaling to a bounds array.
     * @param bounds the bounds (a 6-array).
     * @param logFlags the flags indicating whether inverse scaling must be applied for respectively the x,y,z axes (3 elements).
     */
    public static void applyInverseLogScaleToBounds(Double[] bounds, boolean[] logFlags) {
        if (logFlags[0]) {
            bounds[0] = Math.pow(10, bounds[0]);
            bounds[1] = Math.pow(10, bounds[1]);
        }

        if (logFlags[1]) {
            bounds[2] = Math.pow(10, bounds[2]);
            bounds[3] = Math.pow(10, bounds[3]);
        }

        if (logFlags[2]) {
            bounds[4] = Math.pow(10, bounds[4]);
            bounds[5] = Math.pow(10, bounds[5]);
        }
    }
}
