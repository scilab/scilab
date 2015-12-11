/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
