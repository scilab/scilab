/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.event.MouseEvent;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Bruno JOFRET
 */
public class ZoomRubberBox extends RubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public ZoomRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
    }

    @Override
    public final void mouseClicked(MouseEvent e) {
        mouseButton = e.getButton();
        if (e.getButton() == MouseEvent.BUTTON1) {
            switch (status) {
            case WAIT_POINT_A:
                if (setPointA(e.getPoint())) {
                    status = Status.WAIT_POINT_B;
                } else {
                    setEnable(false);
                    fireRubberBoxEnd();
                }
                break;
            case WAIT_POINT_B:
                setPointB(e.getPoint());
                if (pointBComputer.is2D()) {
                    process();
                    setEnable(false);
                    fireRubberBoxEnd();
                } else {
                    status = Status.WAIT_POINT_C;
                }
                break;
            case WAIT_POINT_C:
                setPointC(e.getPoint());
                status = Status.WAIT_POINT_D;
                break;
            case WAIT_POINT_D:
                setPointD(e.getPoint());
                process();
                setEnable(false);
                fireRubberBoxEnd();
                break;
            default:
            }
            updateInfoMessage();
        }

        if (e.getButton() == MouseEvent.BUTTON3) {
            setEnable(false);
            fireRubberBoxEnd();
        }
    }

    /**
     * Actually set the zoom box depending on current value of firstPoint and secondPoint.
     */
    @Override
    protected void process() {
        Double[] bounds = {
                Math.min(firstPoint.getX(), secondPoint.getX()), Math.max(firstPoint.getX(), secondPoint.getX()),
                Math.min(firstPoint.getY(), secondPoint.getY()), Math.max(firstPoint.getY(), secondPoint.getY()),
                Math.min(firstPoint.getZ(), secondPoint.getZ()), Math.max(firstPoint.getZ(), secondPoint.getZ()),
        };

        if (bounds[0].compareTo(bounds[1]) != 0 && bounds[2].compareTo(bounds[3]) != 0 && bounds[4].compareTo(bounds[5]) != 0) {
            Boolean zoomed = tightZoomBounds(axes, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
            getDrawerVisitor().getCanvas().redraw();
        }
    }
}
