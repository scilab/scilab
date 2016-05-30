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
import java.awt.event.MouseEvent;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointAComputer;

/**
 * @author Bruno JOFRET
 */
public class OnePointRubberBox extends RubberBox implements PointRubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public OnePointRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
    }

    public OnePointRubberBox(DrawerVisitor drawerVisitor, double initialRect[]) {
        super(drawerVisitor);
        status = Status.WAIT_POINT_B;
        setEnable(true);

        AxesZoom axesZoom = new AxesZoom(drawerVisitor.getAxes());
        double[][] factors = axesZoom.axes.getScaleTranslateFactors();
        axesZoom.firstPoint = new Vector3d(initialRect[0] * factors[0][0] + factors[1][0],
                                           initialRect[1] * factors[0][1] + factors[1][1],
                                           0);
        axesZoom.secondPoint = axesZoom.firstPoint;
        axesZoomList.add(axesZoom);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        mouseButton = e.getButton();
        lastPoint = e.getPoint();
        setPointB(e.getPoint());
        process();
        setEnable(false);
        fireRubberBoxEnd();
    }

    /**
     * Set the first point.
     * @param point first point AWT coordinate.
     * @return true if the first point is valid.
     */
    protected boolean setPointB(Point point) {
        AxesZoom axesZoom = getClosestAxesZoom(point);
        if (axesZoom != null) {
            PointAComputer pointComputer = new PointAComputer(axesZoom.axes, point);
            if (pointComputer.isValid()) {
                axesZoom.pointBComputer = pointComputer;
                axesZoom.secondPoint = pointComputer.getPosition();
                return true;
            }
        }
        return false;
    }
}
