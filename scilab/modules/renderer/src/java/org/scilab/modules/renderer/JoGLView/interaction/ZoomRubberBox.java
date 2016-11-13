/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;

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
        for (AxesZoom axesZoom : axesZoomList) {
            Double[] bounds = {
                Math.min(axesZoom.firstPoint.getX(), axesZoom.secondPoint.getX()), Math.max(axesZoom.firstPoint.getX(), axesZoom.secondPoint.getX()),
                Math.min(axesZoom.firstPoint.getY(), axesZoom.secondPoint.getY()), Math.max(axesZoom.firstPoint.getY(), axesZoom.secondPoint.getY()),
                Math.min(axesZoom.firstPoint.getZ(), axesZoom.secondPoint.getZ()), Math.max(axesZoom.firstPoint.getZ(), axesZoom.secondPoint.getZ()),
            };

            if (bounds[0].compareTo(bounds[1]) != 0 && bounds[2].compareTo(bounds[3]) != 0 && bounds[4].compareTo(bounds[5]) != 0) {
                Boolean zoomed = tightZoomBounds(axesZoom.axes, bounds);
                double[][] factors = axesZoom.axes.getScaleTranslateFactors();
                bounds[0] = (bounds[0] - factors[1][0]) / factors[0][0];
                bounds[1] = (bounds[1] - factors[1][0]) / factors[0][0];
                bounds[2] = (bounds[2] - factors[1][1]) / factors[0][1];
                bounds[3] = (bounds[3] - factors[1][1]) / factors[0][1];
                bounds[4] = (bounds[4] - factors[1][2]) / factors[0][2];
                bounds[5] = (bounds[5] - factors[1][2]) / factors[0][2];

                boolean[] logFlags = { axesZoom.axes.getXAxisLogFlag(), axesZoom.axes.getYAxisLogFlag(), axesZoom.axes.getZAxisLogFlag()};
                ScaleUtils.applyInverseLogScaleToBounds(bounds, logFlags);

                GraphicController.getController().setProperty(axesZoom.axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                GraphicController.getController().setProperty(axesZoom.axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                getDrawerVisitor().getCanvas().redraw();
            }
        }
    }
}
