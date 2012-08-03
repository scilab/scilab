/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction.util;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;

import java.awt.Point;

/**
 * @author Pierre Lando
 */
public class PointCComputer extends CubeFacesPointComputer implements PointComputer {

    private final Vector3d secondPosition;
    private final Vector3d firstPosition;
    private final int firstAxisIndex;


    /**
     * Constructor
     *
     * @param axes current axes.
     * @param pointBComputer previous point computer.
     * @param point clicked point in AWT coordinate.
     */
    public PointCComputer(Axes axes, PointComputer pointBComputer, Point point) {
        super(axes, point);
        firstAxisIndex = pointBComputer.getFirstAxisIndex();
        if (getPosition() != null) {
            firstPosition = setCoordinate(pointBComputer.getFirstPosition(), getPosition(), firstAxisIndex);
            secondPosition = setCoordinate(pointBComputer.getSecondPosition(), getPosition(), firstAxisIndex);
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

    @Override
    public boolean is2D() {
        return false;
    }
}
