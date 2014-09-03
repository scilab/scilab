/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.Point;
import java.awt.event.MouseEvent;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
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
        double[][] factors = axes.getScaleTranslateFactors();
        firstPoint = new Vector3d(initialRect[0] * factors[0][0] + factors[1][0],
                initialRect[1] * factors[0][1] + factors[1][1],
                0);
        secondPoint = firstPoint;
        status = Status.WAIT_POINT_B;
        setEnable(true);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        mouseButton = e.getButton();
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
        axes = getUnderlyingAxes(point);
        if (axes != null) {
            PointAComputer pointComputer = new PointAComputer(axes, point);
            if (pointComputer.isValid()) {
                pointBComputer = pointComputer;
                secondPoint = pointComputer.getPosition();
                return true;
            }
        }
        return false;
    }
}
