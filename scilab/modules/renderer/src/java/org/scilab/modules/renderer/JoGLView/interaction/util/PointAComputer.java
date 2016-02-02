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
