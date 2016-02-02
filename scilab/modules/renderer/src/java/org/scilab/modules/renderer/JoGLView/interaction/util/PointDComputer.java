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
public class PointDComputer extends CubeFacesPointComputer implements PointComputer {

    private final int firstAxisIndex;
    private final Vector3d secondPosition;
    private final Vector3d firstPosition;

    /**
     * Constructor
     *
     * @param axes current axes.
     * @param pointCComputer previous point computer.
     * @param point clicked point in AWT coordinate.
     */
    public PointDComputer(Axes axes, PointComputer pointCComputer, Point point) {
        super(axes, point);
        firstAxisIndex = pointCComputer.getFirstAxisIndex();
        if (getPosition() != null) {
            firstPosition = pointCComputer.getFirstPosition();
            secondPosition = setCoordinate(pointCComputer.getSecondPosition(), getPosition(), firstAxisIndex);
        } else {
            firstPosition = null;
            secondPosition = null;
        }
    }

    @Override
    public final boolean isValid() {
        return secondPosition != null;
    }

    @Override
    public final Vector3d getFirstPosition() {
        return firstPosition;
    }

    @Override
    public final Vector3d getSecondPosition() {
        return secondPosition;
    }

    @Override
    public final int getFirstAxisIndex() {
        return firstAxisIndex;
    }

    @Override
    public boolean is2D() {
        return false;
    }
}
