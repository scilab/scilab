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

import java.awt.event.MouseEvent;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Bruno JOFRET
 */
public class DragPointRubberBox extends TwoPointsRubberBox {

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    public DragPointRubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
    }

    @Override
    public final void mouseClicked(MouseEvent e) {

    }
    @Override
    public void mousePressed(MouseEvent e) {
        mouseButton = e.getButton();
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

    @Override
    public void mouseReleased(MouseEvent e) {
        mouseButton = e.getButton();
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

    @Override
    public final void mouseDragged(MouseEvent e) {
        switch (status) {
            case WAIT_POINT_A:
                setPointA(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_B:
                setPointB(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_C:
                setPointC(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            case WAIT_POINT_D:
                setPointD(e.getPoint());
                getDrawerVisitor().getCanvas().redraw();
                break;
            default:
        }
        updateInfoMessage();
    }


}
