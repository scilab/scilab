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
                if (!valid3D()) {
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
        updateInfoMessage(e.getPoint());
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
                if (!valid3D()) {
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
        updateInfoMessage(e.getPoint());
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
        updateInfoMessage(e.getPoint());
    }


}
