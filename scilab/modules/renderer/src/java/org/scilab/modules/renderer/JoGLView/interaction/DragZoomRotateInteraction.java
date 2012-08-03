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
package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import org.scilab.modules.commons.OS;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * This class manage figure interaction.
 *
 * @author Pierre Lando
 */
public class DragZoomRotateInteraction extends FigureInteraction {

    private static final int XY_TRANSLATION_MODIFIER = MouseEvent.BUTTON1_MASK;
    private static final int Z_TRANSLATION_MODIFIER = MouseEvent.BUTTON1_MASK | MouseEvent.ALT_MASK;
    private static final int ROTATION_MODIFIER = MouseEvent.BUTTON3_MASK;
    private static final int MACOSX_ROTATION_MODIFIER = MouseEvent.BUTTON1_MASK | MouseEvent.CTRL_MASK;

    /**
     * The box size is multiply by this value.
     */
    private static final double ZOOM_FACTOR = 1.02;

    private final MouseListener mouseListener;
    private final MouseWheelListener mouseWheelListener;
    private final MouseMotionListener mouseMotionListener;

    /**
     * Last important mouse event.
     */
    private MouseEvent previousEvent;
    private Axes currentAxes;

    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public DragZoomRotateInteraction(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        mouseMotionListener = new FigureMouseMotionListener();
        mouseWheelListener = new FigureMouseWheelListener();
        mouseListener = new FigureMouseListener();
    }

    @Override
    protected void changeEnable(boolean isEnable) {
        Component component = getDrawerVisitor().getComponent();
        if (component != null) {
            if (isEnable) {
                component.addMouseListener(mouseListener);
                component.addMouseWheelListener(mouseWheelListener);
            } else {
                component.removeMouseListener(mouseListener);
                component.removeMouseMotionListener(mouseMotionListener);
                component.removeMouseWheelListener(mouseWheelListener);
            }
        }
    }

    /**
     * This {@see MouseListner} activate the {@see MouseMotionListener} when at least
     * one button is pressed.
     * The event is saved in {@see previousEvent}
     */
    private class FigureMouseListener extends MouseAdapter implements MouseListener {

        private int pressedButtons = 0;

        @Override
        public void mousePressed(MouseEvent e) {
            if (pressedButtons == 0) {
                previousEvent = e;
                if (currentAxes == null) {
                    currentAxes = getUnderlyingAxes(e.getPoint());
                    if (currentAxes != null) {
                        getDrawerVisitor().getComponent().addMouseMotionListener(mouseMotionListener);
                    }
                }
            }
            pressedButtons++;
        }

        @Override
        public void mouseReleased(MouseEvent e) {
            pressedButtons--;
            if (pressedButtons == 0) {
                getDrawerVisitor().getComponent().removeMouseMotionListener(mouseMotionListener);
                currentAxes = null;
            }
        }
    }

    /**
     * This {@see MouseWheelListener} manage zoom/un-zoom on the figure.
     */
    private class FigureMouseWheelListener implements MouseWheelListener {

        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            Axes axes = getUnderlyingAxes(e.getPoint());
            if (axes != null) {
                double scale = Math.pow(ZOOM_FACTOR, e.getUnitsToScroll());
                Double[] bounds = axes.getDisplayedBounds();

                double xDelta = (bounds[1] - bounds[0]) / 2;
                double xMiddle = (bounds[1] + bounds[0]) / 2;
                bounds[0] = xMiddle - xDelta * scale;
                bounds[1] = xMiddle + xDelta * scale;

                double yDelta = (bounds[3] - bounds[2]) / 2;
                double yMiddle = (bounds[3] + bounds[2]) / 2;
                bounds[2] = yMiddle - yDelta * scale;
                bounds[3] = yMiddle + yDelta * scale;

                double zDelta = (bounds[5] - bounds[4]) / 2;
                double zMiddle = (bounds[5] + bounds[4]) / 2;
                bounds[4] = zMiddle - zDelta * scale;
                bounds[5] = zMiddle + zDelta * scale;

                Boolean zoomed = tightZoomBounds(axes, bounds);
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
            }
        }
    }

    /**
     * This {@see MouseMotionListener} manage rotation and translation on the figure.
     */
    private class FigureMouseMotionListener extends MouseMotionAdapter implements MouseMotionListener {

        @Override
        public void mouseMoved(MouseEvent e) {
            /*
             * Mac OS X specific case: the users first presses CTRL and then left-clic.
             */
            if (OS.get() == OS.MAC && e.isControlDown() && e.getButton() == 0) {
                doRotation(e);
            }
        }
        public void mouseDragged(MouseEvent e) {
            switch (e.getModifiers()) {
                case MACOSX_ROTATION_MODIFIER:
                    /*
                     * Mac OS X specific case: the users first left-clic and then presses CTRL
                     */
                    if (OS.get() == OS.MAC && e.isControlDown()) {
                        doRotation(e);
                        break;
                    }
                case XY_TRANSLATION_MODIFIER:
                    doXYTranslation(e);
                    break;
                case Z_TRANSLATION_MODIFIER:
                    doZTranslation(e);
                    break;
                case ROTATION_MODIFIER:
                    doRotation(e);
                    break;
            }

            previousEvent = e;
        }

        private void doRotation(MouseEvent e) {
            int dx = e.getX() - previousEvent.getX();
            int dy = e.getY() - previousEvent.getY();

            if (currentAxes != null) {
                Double[] angles = currentAxes.getRotationAngles();
                angles[0] -= dy / 4.0;
                angles[1] -= Math.signum(Math.sin(Math.toRadians(angles[0]))) * (dx / 4.0);
                GraphicController.getController().setProperty(currentAxes.getIdentifier(), GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
            }
        }

        private void doXYTranslation(MouseEvent e) {
            int dx = e.getX() - previousEvent.getX();
            int dy = e.getY() - previousEvent.getY();

            if (currentAxes != null) {
                if (currentAxes.getZoomEnabled()) {
                    Double[] bounds = currentAxes.getDisplayedBounds();
                    double orientation = Math.signum(Math.cos(Math.toRadians(currentAxes.getRotationAngles()[0])));
                    double angle = - orientation * Math.toRadians(currentAxes.getRotationAngles()[1]);

                    double xDelta = (bounds[0] - bounds[1]) / 100;
                    double yDelta = (bounds[2] - bounds[3]) / 100;

                    double rotatedDX = dx * Math.sin(angle) + dy * Math.cos(angle);
                    double rotatedDY = dx * Math.cos(angle) - dy * Math.sin(angle);

                    bounds[0] += xDelta * rotatedDX * orientation;
                    bounds[1] += xDelta * rotatedDX * orientation;

                    bounds[2] += yDelta * rotatedDY;
                    bounds[3] += yDelta * rotatedDY;

                    Boolean zoomed = tightZoomBoxToDataBounds(currentAxes, bounds);
                    GraphicController.getController().setProperty(currentAxes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                    GraphicController.getController().setProperty(currentAxes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                }
            }
        }

        private void doZTranslation(MouseEvent e) {
            int dy = e.getY() - previousEvent.getY();

            if (currentAxes != null) {
                Double[] bounds = currentAxes.getDisplayedBounds();

                double zDelta = (bounds[5] - bounds[4]) / 100;

                bounds[4] += zDelta * dy;
                bounds[5] += zDelta * dy;

                Boolean zoomed = tightZoomBoxToDataBounds(currentAxes, bounds);
                GraphicController.getController().setProperty(currentAxes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                GraphicController.getController().setProperty(currentAxes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
            }
        }

        /**
         * Tight given bounds to axes data bounds.
         * Bounds length along axes are conserved.
         * @param axes the given axes.
         * @param zoomBounds the zoomBounds.
         * @return true if actually there is a zoom.
         */
        private boolean tightZoomBoxToDataBounds(Axes axes, Double[] zoomBounds) {
            boolean zoomed = false;
            Double[] dataBounds = axes.getMaximalDisplayedBounds();
            for (int i : new int[] {0, 2, 4}) {
                if (zoomBounds[i] < dataBounds[i]) {
                    double delta = dataBounds[i] - zoomBounds[i];
                    zoomBounds[i] = dataBounds[i]; // zoomBounds[i] += delta;
                    zoomBounds[i + 1] += delta;
                } else {
                    zoomed = true;
                }
            }

            for (int i : new int[] {1, 3, 5}) {
                if (zoomBounds[i] > dataBounds[i]) {
                    double delta = dataBounds[i] - zoomBounds[i];
                    zoomBounds[i] = dataBounds[i]; // zoomBounds[i] += delta;
                    zoomBounds[i - 1] += delta;
                } else {
                    zoomed = true;
                }
            }

            return zoomed;
        }
    }
}
