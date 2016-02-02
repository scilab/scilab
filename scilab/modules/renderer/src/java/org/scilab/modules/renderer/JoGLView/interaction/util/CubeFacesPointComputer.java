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

        int index = -1;

        // axis represent X, Y and Z axis.
        for (int axis = 0; axis < AXIS_NUMBER; axis++) {
            for (int boundIndex = 0; boundIndex < 2; boundIndex++) {
                double bound = bounds[axis * 2 + boundIndex];
                double lambda = computeLambda(bound, axis);
                if (lambda > maxLambda) {
                    Vector3d v = computeCoordinate(lambda, bound, axis);
                    if (inBounds(v)) {
                        maxLambda = lambda;
                        returnedValue = v;
                        index = axis;
                    }
                }
            }
        }

        this.axisIndex = index;

        position = returnedValue;
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
