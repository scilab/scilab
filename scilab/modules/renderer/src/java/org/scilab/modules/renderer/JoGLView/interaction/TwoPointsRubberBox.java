/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.Point;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointAComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointBComputer;

/**
 * @author Bruno JOFRET
 */
public class TwoPointsRubberBox extends RubberBox implements PointRubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public TwoPointsRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
    }

    /**
     * Set the first point.
     * @param point first point AWT coordinate.
     * @return true if the first point is valid.
     */
    @Override
    protected boolean setPointA(Point point) {
        axesZoomList.clear();
        boolean status = false;
        Axes axes = getUnderlyingAxes(point);
        if (axes != null) {
            AxesZoom axesZoom = new AxesZoom(axes);
            PointAComputer pointComputer = new PointAComputer(axes, point);
            if (pointComputer.isValid()) {
                axesZoom.pointAComputer = pointComputer;
                axesZoom.firstPoint = pointComputer.getPosition();
                axesZoom.secondPoint = axesZoom.firstPoint;
                axesZoomList.add(axesZoom);
                status = true;
            }
        }
        return status;
    }
}
