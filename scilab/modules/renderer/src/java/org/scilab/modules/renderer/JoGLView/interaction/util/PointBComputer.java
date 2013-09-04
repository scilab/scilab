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
public class PointBComputer extends AbstractPointComputer implements PointComputer {

    /** First axis index */
    private final int firstAxisIndex;

    private final boolean is2D;

    private Vector3d firstPosition;
    private Vector3d secondPosition;

    /**
     * Constructor.
     *
     * @param axes current Axes.
     * @param pointAComputer the first point computer.
     * @param point the clicked point in AWT coordinate.
     */
    public PointBComputer(Axes axes, PointComputer pointAComputer, Point point) {
        super(axes, point);

        firstAxisIndex = pointAComputer.getFirstAxisIndex();
        if (firstAxisIndex != -1) {
            double value = pointAComputer.getFirstPosition().getData()[firstAxisIndex];
            double lambda = computeLambda(value, firstAxisIndex);
            Vector3d coordinate = computeCoordinate(lambda, value, firstAxisIndex);

            firstPosition = pointAComputer.getFirstPosition();
            secondPosition = clamp(coordinate);

            if (check2D()) {
                double[] data = firstPosition.getData();
                data[firstAxisIndex] = -Double.MAX_VALUE;
                firstPosition = clamp(new Vector3d(data));

                data = secondPosition.getData();
                data[firstAxisIndex] = Double.MAX_VALUE;
                secondPosition = clamp(new Vector3d(data));

                is2D = true;
            } else {
                is2D = false;
            }

        } else {
            is2D = false;
            firstPosition = null;
            secondPosition = null;
        }
    }

    /**
     * Check if the current axes disposition lead to a 2D zoom.
     * @return true if the current axes disposition lead to a 2D zoom.
     */
    private boolean check2D() {
        boolean r = true;
        Double[] bounds = getAxes().getDataBounds();
        float[] middle = new float[] {
            (float) (bounds[0] + bounds[1]) / 2f,
            (float) (bounds[2] + bounds[3]) / 2f,
            (float) (bounds[4] + bounds[5]) / 2f
        };

        for (int i = 0; i < AXIS_NUMBER; i++) {
            if (i != firstAxisIndex) {
                double lambda = computeLambda(middle[i], i);
                r &= !((lambda >= 0) && (lambda <= 1));
            }
        }

        return r;
    }

    @Override
    public final boolean isValid() {
        return secondPosition != null;
    }

    /**
     * 2D status getter.
     * @return true if the zoom is in 2D mode.
     */
    public final boolean is2D() {
        return is2D;
    }

    @Override
    public final int getFirstAxisIndex() {
        return firstAxisIndex;
    }

    @Override
    public final Vector3d getFirstPosition() {
        return firstPosition;
    }

    @Override
    public final Vector3d getSecondPosition() {
        return secondPosition;
    }
}
