/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView.interaction.util;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;

import java.awt.Point;

/**
 * @author Pierre Lando
 */
public abstract class AbstractPointComputer implements PointComputer {

    /** Closest projected point */
    private final Vector3d min;

    /** Far projected point */
    private final Vector3d max;

    /** Current Axes */
    private final Axes axes;

    /**
     * Default constructor.
     * @param axes current axes.
     * @param point clicked point in AWT coordinate.
     */
    public AbstractPointComputer(Axes axes, Point point) {
        this.axes = axes;
        min = AxesDrawer.unProject(axes, new Vector3d(point.x, point.y, -1));
        max = AxesDrawer.unProject(axes, new Vector3d(point.x, point.y, +1));
    }

    /**
     * Axes getter.
     * @return the current axes.
     */
    protected final Axes getAxes() {
        return axes;
    }

    /**
     * Compute lambda such that:
     *  lambda * min[axisIndex] + (1 - lambda) * max[axisIndex] = value.
     * @param value given value.
     * @param axisIndex given axisIndex.
     * @return lambda.
     */
    protected final double computeLambda(double value, int axisIndex) {
        double v1 = min.getData()[axisIndex];
        double v2 = max.getData()[axisIndex];
        return (value - v2) / (v1 - v2);
    }

    /**
     * Compute v such that:
     *  v = lambda * min + (1 - lambda) * max
     *
     *  With lambda such that:
     *  v[axisIndex] = value.
     *
     *  It always true that v[axisIndex] = value.
     * @param lambda given lambda.
     * @param value given value.
     * @param axisIndex given axisIndex.
     * @return v.
     */
    protected final Vector3d computeCoordinate(double lambda, double value, int axisIndex) {
        double[] data = min.times(lambda).plus(max.times(1 - lambda)).getData();
        data[axisIndex] = value;
        return new Vector3d(data);
    }

    /**
     * Check that given coordinate vector feet axes bounds.
     * @param v given vector.
     * @return true if v feet in the bounds.
     */
    protected final boolean inBounds(Vector3d v) {
        Double[] bounds = axes.getCorrectedBounds();
        double[] data = v.getData();
        for (int i = 0; i < AXIS_NUMBER; i++) {
            if ((bounds[i * 2] > data[i]) || (bounds[i * 2 + 1] < data[i])) {
                return false;
            }
        }
        return true;
    }

    /**
     * Check if the given vector has valid components
     * not NaN and not Inf.
     *
     * @param v the 3d vector
     * @return true if all components are valid, false otherwise
     */
    protected final boolean isValid(Vector3d v) {
        double[] data = v.getData();
        for (int i = 0; i < AXIS_NUMBER; i++) {
            if (Double.isNaN(data[i]) || Double.isInfinite(data[i])) {
                return false;
            }
        }
        return true;
    }


    /**
     * Check if the given position feet axes bounds.
     * @param position the given position.
     * @return true if it feet.
     */
    protected final Vector3d clamp(Vector3d position) {
        Double[] bounds = axes.getCorrectedBounds();
        double[] data = position.getData();
        for (int i = 0; i < AXIS_NUMBER; i++) {
            if (data[i] < bounds[i * 2]) {
                data[i] = bounds[i * 2];
            } else if (data[i] > bounds[i * 2 + 1]) {
                data[i] = bounds[i * 2 + 1];
            }
        }
        return new Vector3d(data);
    }

    /**
     * Compute the first vector with the i-th value set to the one from the second vector.
     * @param v1 first vector.
     * @param v2 second vector.
     * @param i coordinate index.
     * @return return the {@see clamp} value of computed vector.
     */
    protected final Vector3d setCoordinate(Vector3d v1, Vector3d v2, int i) {
        double[] data = v1.getData();
        data[i] = v2.getData()[i];
        return clamp(new Vector3d(data));
    }
}
