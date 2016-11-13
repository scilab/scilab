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

import java.awt.Point;

/**
 * @author Pierre Lando
 */
public abstract class CubeFacesPointComputer extends AbstractPointComputer {

    /** Clicked position */
    private final Vector3d position;

    /** First axis index */
    private final int axisIndex;

    private static final double threshold = 0.05;

    /**
     * Constructor.
     *
     * @param axes box axes.
     * @param point screen point.
     */
    public CubeFacesPointComputer(final Axes axes, final Point point) {
        super(axes, point);
        Double[] bounds = axes.getCorrectedBounds();

        double maxLambda = -Double.MAX_VALUE;
        Vector3d returnedValue = null;

        Vector3d lastValidClamppedValue = null;
        double lasValidMinLambda = Double.MAX_VALUE;
        int lastValidIndex = -1;

        int index = -1;

        // axis represent X, Y and Z axis.
        for (int axis = 0; axis < AXIS_NUMBER; axis++) {

            double lastBoundLambda = -Double.MAX_VALUE;
            int lastBoundIndex = -1;
            Vector3d lastBoundClamppedValue = null;

            for (int boundIndex = 0; boundIndex < 2; boundIndex++) {
                double bound = bounds[axis * 2 + boundIndex];
                double lambda = computeLambda(bound, axis);
                if (lambda > maxLambda) {
                    Vector3d v = computeCoordinate(lambda, bound, axis);
                    if (inBounds(v)) {
                        maxLambda = lambda;
                        returnedValue = v;
                        index = axis;
                    } else if (isValid(v) && lambda > lastBoundLambda) {
                        lastBoundLambda = lambda;
                        lastBoundClamppedValue = clamp(v);
                        lastBoundIndex = axis;
                    }
                }
            }
            if (lasValidMinLambda > lastBoundLambda || check2D()) {
                lasValidMinLambda = lastBoundLambda;
                lastValidIndex = lastBoundIndex;
                lastValidClamppedValue = lastBoundClamppedValue;
            }
        }

        if (returnedValue != null) {
            this.axisIndex = index;
            position = check2D() ? returnedValue : applyThreshold(returnedValue);
        } else {
            this.axisIndex = lastValidIndex;
            position = check2D() ? lastValidClamppedValue : applyThreshold(lastValidClamppedValue);
        }
    }

    /**
     * Checks whether the computated lambdas
     * are from a 2d axes view or not.
     *
     * @return true if 2d view, false otherwise.
     **/
    private boolean check2D() {
        int r = 0;
        Double[] bounds = getAxes().getDataBounds();
        float[] middle = new float[] {
            (float) (bounds[0] + bounds[1]) / 2f,
            (float) (bounds[2] + bounds[3]) / 2f,
            (float) (bounds[4] + bounds[5]) / 2f
        };

        for (int i = 0; i < AXIS_NUMBER; i++) {
            double lambda = computeLambda(middle[i], i);
            r = !((lambda >= 0) && (lambda <= 1)) ? r + 1 : r;
        }
        return (r >= 2);
    }

    /**
     * Checks if the components of the given vector are
     * too close to -1.0 or 1.0 and rounds it according.
     *
     * @param v the 3d vector with components [-1.0 , 1.0]
     * @return 3d vector
     **/
    private Vector3d applyThreshold(Vector3d v) {
        double[] data = v.getData();
        data[0] =  data[0] < 0.0 ? ((data[0] - threshold) < -1.0 ? -1.0 : data[0]) : ((data[0] + threshold) > 1.0 ? 1.0 : data[0]);
        data[1] =  data[1] < 0.0 ? ((data[1] - threshold) < -1.0 ? -1.0 : data[1]) : ((data[1] + threshold) > 1.0 ? 1.0 : data[1]);
        data[2] =  data[2] < 0.0 ? ((data[2] - threshold) < -1.0 ? -1.0 : data[2]) : ((data[2] + threshold) > 1.0 ? 1.0 : data[2]);
        return new Vector3d(data);
    }

    /**
     * Axis index getter.
     * @return the current axis index.
     */
    protected final int getAxisIndex() {
        return axisIndex;
    }

    /**
     * Clicked position getter.
     * @return the position.
     */
    public final Vector3d getPosition() {
        return position;
    }
}
