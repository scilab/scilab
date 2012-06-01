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
 * This class compute the coordinate of a point on the box surface.
 * The point correspond to a mouse coordinate.
 * @author Pierre Lando
 */
public class PointAComputer extends CubeFacesPointComputer implements PointComputer {

    /**
     * Constructor.
     *
     * @param axes  box axes.
     * @param point screen point.
     */
    public PointAComputer(final Axes axes, final Point point) {
        super(axes, point);
    }

    @Override
    public final boolean isValid() {
        return getPosition() != null;
    }

    @Override
    public final int getFirstAxisIndex() {
        return getAxisIndex();
    }

    @Override
    public final Vector3d getFirstPosition() {
        return getPosition();
    }

    @Override
    public final Vector3d getSecondPosition() {
        return getPosition();
    }

    @Override
    public boolean is2D() {
        return false;
    }
}
