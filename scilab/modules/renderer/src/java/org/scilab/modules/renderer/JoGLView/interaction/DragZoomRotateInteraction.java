/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2018 - Stéphane MOTTELET
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

import java.awt.Component;
import java.awt.Cursor;
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
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.renderer.utils.EntityPicker;
import org.scilab.modules.renderer.utils.EntityPicker.SurfaceInfo;


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
    private Axes[] currentAxes;


    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public DragZoomRotateInteraction(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        mouseMotionListener = new FigureMouseMotionListener();
        mouseWheelListener = new FigureMouseWheelListener();
        mouseListener = new FigureMouseListener();
        currentAxes = new Axes[0];
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

    public void setTranslationEnable(boolean status) {
        ((FigureMouseMotionListener)mouseMotionListener).setTranslateEnable(status);
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
                if (currentAxes.length == 0) {
                    currentAxes = getAllUnderlyingAxes(e.getPoint());
                    if (currentAxes.length > 0) {
                        getDrawerVisitor().getComponent().addMouseMotionListener(mouseMotionListener);
                        switch (e.getButton()) {
                            case MouseEvent.BUTTON1 :
                                Cursor cursor = Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
                                e.getComponent().setCursor(cursor);
                                break;
                            case MouseEvent.BUTTON3 :
                                // FIXME: add rotation cursor here
                                break;
                        }
                    }
                }
            }
            pressedButtons++;
        }

        @Override
        public void mouseReleased(MouseEvent e) {
            if (pressedButtons > 0) {
                pressedButtons--;
            }

            if (pressedButtons == 0) {
                getDrawerVisitor().getComponent().removeMouseMotionListener(mouseMotionListener);
                currentAxes = new Axes[0];
            }
            e.getComponent().setCursor(Cursor.getDefaultCursor());
        }
    }

    /**
     * This {@see MouseWheelListener} manage zoom/un-zoom on the figure.
     */
    private class FigureMouseWheelListener implements MouseWheelListener {

        private void applyZoom(Axes axes, double scale, double[] position) {
            if (axes != null) {
                Double[] bounds = axes.getDisplayedBounds();
                double[][] factors = axes.getScaleTranslateFactors();
                // Zoom only if position of mouse cursor is inside the bounds
                if (position[0] >= bounds[0] && position[0] <= bounds[1] &&
                        position[1] >= bounds[2] && position[1] <= bounds[3] &&
                        position[2] >= bounds[4] && position[2] <= bounds[5]) {
                    bounds[0] = position[0] + (bounds[0] - position[0]) * scale;
                    bounds[1] = position[0]  + (bounds[1] - position[0]) * scale;
                    bounds[2] = position[1] + (bounds[2] - position[1]) * scale;
                    bounds[3] = position[1] + (bounds[3] - position[1]) * scale;
                    if (axes.getView() != 0) {
                        bounds[4] = position[2] + (bounds[4] - position[2]) * scale;
                        bounds[5] = position[2] + (bounds[5] - position[2]) * scale;
                    }
                    bounds[0] = bounds[0] * factors[0][0] + factors[1][0];
                    bounds[1] = bounds[1] * factors[0][0] + factors[1][0];
                    bounds[2] = bounds[2] * factors[0][1] + factors[1][1];
                    bounds[3] = bounds[3] * factors[0][1] + factors[1][1];
                    bounds[4] = bounds[4] * factors[0][2] + factors[1][2];
                    bounds[5] = bounds[5] * factors[0][2] + factors[1][2];

                    Boolean zoomed = tightZoomBounds(axes, bounds);

                    bounds[0] = (bounds[0] - factors[1][0]) / factors[0][0];
                    bounds[1] = (bounds[1] - factors[1][0]) / factors[0][0];
                    bounds[2] = (bounds[2] - factors[1][1]) / factors[0][1];
                    bounds[3] = (bounds[3] - factors[1][1]) / factors[0][1];
                    bounds[4] = (bounds[4] - factors[1][2]) / factors[0][2];
                    bounds[5] = (bounds[5] - factors[1][2]) / factors[0][2];

                    boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
                    ScaleUtils.applyInverseLogScaleToBounds(bounds, logFlags);

                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                }
            }
        }
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            Axes[] allAxes;
            if (e.isControlDown()) {
                allAxes = getAllVisibleAxes(e.getPoint());
            } else {
                allAxes = getAllUnderlyingAxes(e.getPoint());
            }
            double scale = Math.pow(ZOOM_FACTOR, e.getUnitsToScroll());
            double[] position = null;
            for (Axes axes : allAxes) {
                // beware: components of axes.getDisplayedBounds() are log10 of bounds
                // when axes.get[X,Y or Z]AxisLogFlag() is true
                Double[] bounds = axes.getDisplayedBounds();
                Double[] realBounds = axes.getDisplayedBounds();
                boolean[] logFlags = {axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
                ScaleUtils.applyInverseLogScaleToBounds(realBounds, logFlags);
                // If possible, center the homothecy at a point of a polyline or a point of a surface
                EntityPicker ep = new EntityPicker();
                // TODO: picking info should give the Z in order to take the closest object
                // between polyline and surface
                Integer polylineUid = ep.pick(axes.getParentFigure(), e.getX(), e.getY());
                SurfaceInfo surfInfo = ep.pickSurface(axes.getParentFigure(), new Integer[] {e.getX(), e.getY()});
                if (polylineUid != null) {
                    EntityPicker.PickedPoint picked = ep.pickPoint(polylineUid, e.getX(), e.getY());
                    double[] datax = (double[])PolylineData.getDataX(polylineUid);
                    double[] datay = (double[])PolylineData.getDataY(polylineUid);
                    double[] dataz = (double[])PolylineData.getDataZ(polylineUid);
                    // in 3D ep.pick (for PolyLines) does not check that picked point is in the viewing box
                    if (datax[picked.point] > realBounds[0] && datax[picked.point] < realBounds[1] &&
                            datay[picked.point] > realBounds[2] && datay[picked.point] < realBounds[3] &&
                            dataz[picked.point] > realBounds[4] && dataz[picked.point] < realBounds[5]) {
                        position = new double[] {datax[picked.point], datay[picked.point], dataz[picked.point]};
                    }
                } else if (surfInfo != null && surfInfo.point != null) {
                    position = new double[] {surfInfo.point.getX(), surfInfo.point.getY(), surfInfo.point.getZ()};
                }
                if (position == null) {
                    if (axes.getView() == 0) {
                        // 2D : failsafe homothecy centered at mouse coordinates
                        double[] pixelPosition = {e.getX(), e.getY(), 0};
                        position = CallRenderer.get2dViewFromPixelCoordinates(axes.getIdentifier(), pixelPosition);
                        position[2] = (bounds[4] + bounds[5]) / 2;
                    } else {
                        // 3D : failsafe homothecy centered at center of viewing box
                        position = new double[] {(bounds[1] + bounds[0]) / 2, (bounds[3] + bounds[2]) / 2, (bounds[5] + bounds[4]) / 2};
                    }
                }
                // the zomm is applied in the linear scale, hence coordinates have to be tranformed accordingly
                ScaleUtils.applyLogScale(position, logFlags);
                applyZoom(axes, scale, position);
            }
        }
    }

    private static void applyUnlog(Double[] bounds, Axes axes) {
        if (axes.getXAxisLogFlag()) {
            bounds[0] = Math.pow(10, bounds[0]);
            bounds[1] = Math.pow(10, bounds[1]);
        }

        if (axes.getYAxisLogFlag()) {
            bounds[2] = Math.pow(10, bounds[2]);
            bounds[3] = Math.pow(10, bounds[3]);
        }

        if (axes.getZAxisLogFlag()) {
            bounds[4] = Math.pow(10, bounds[4]);
            bounds[5] = Math.pow(10, bounds[5]);
        }
    }

    /**
     * This {@see MouseMotionListener} manage rotation and translation on the figure.
     */
    private class FigureMouseMotionListener extends MouseMotionAdapter implements MouseMotionListener {

        private boolean translateEnabled = true;

        public void setTranslateEnable(boolean status) {
            translateEnabled = status;
        }

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
                    break;
                case XY_TRANSLATION_MODIFIER:
                    if (translateEnabled) {
                        doXYTranslation(e);
                    }
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

            for (Axes axes : currentAxes) {
                if (axes.getView() != 0) {
                    Double[] angles = axes.getRotationAngles();
                    angles[0] -= dy / 4.0;
                    angles[1] -= Math.signum(Math.sin(Math.toRadians(angles[0]))) * (dx / 4.0);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
                }
            }
        }

        private void doXYTranslation(MouseEvent e) {
            int dx = e.getX() - previousEvent.getX();
            int dy = e.getY() - previousEvent.getY();

            for (Axes axes : currentAxes) {
                if (axes.getZoomEnabled()) {
                    Double[] bounds = axes.getDisplayedBounds();

                    Integer[] winSize = (Integer[]) GraphicController.getController().getProperty(axes.getParent(), GraphicObjectProperties.__GO_AXES_SIZE__);
                    if (winSize == null) {
                        // We are in a Frame
                        Double[] position = (Double[]) GraphicController.getController().getProperty(axes.getParent(), GraphicObjectProperties.__GO_POSITION__);
                        winSize = new Integer[2];
                        winSize[0] = position[2].intValue();
                        winSize[1] = position[3].intValue();
                    }
                    Double[] axesBounds = (Double[]) GraphicController.getController().getProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_AXES_BOUNDS__);
                    Double[] axesMargins = (Double[]) GraphicController.getController().getProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_MARGINS__);
                    Integer view = (Integer) GraphicController.getController().getProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_VIEW__);

                    // Compute ratio from pixel move to user displayed data bounds
                    double xDelta = Math.abs(bounds[0] - bounds[1]) / (winSize[0] * axesBounds[2] * (1 - axesMargins[0] - axesMargins[1]));
                    double yDelta = Math.abs(bounds[2] - bounds[3]) / (winSize[1] * axesBounds[3] * (1 - axesMargins[2] - axesMargins[3]));

                    if (view == 0) {
                        // 2D View
                        bounds[0] -= xDelta * dx;
                        bounds[1] -= xDelta * dx;

                        bounds[2] += yDelta * dy;
                        bounds[3] += yDelta * dy;
                    } else {
                        // 3D view
                        double orientation = - Math.signum(Math.cos(Math.toRadians(axes.getRotationAngles()[0])));
                        double angle = - orientation * Math.toRadians(axes.getRotationAngles()[1]);

                        double rotatedDX = dx * Math.sin(angle) + dy * Math.cos(angle);
                        double rotatedDY = dx * Math.cos(angle) - dy * Math.sin(angle);

                        bounds[0] -= xDelta * rotatedDX * orientation;
                        bounds[1] -= xDelta * rotatedDX * orientation;

                        bounds[2] += yDelta * rotatedDY;
                        bounds[3] += yDelta * rotatedDY;
                    }

                    Boolean zoomed = tightZoomBoxToDataBounds(axes, bounds);
                    boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
                    ScaleUtils.applyInverseLogScaleToBounds(bounds, logFlags);

                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
                }
            }
        }

        private void doZTranslation(MouseEvent e) {
            int dy = e.getY() - previousEvent.getY();

            for (Axes axes : currentAxes) {
                Double[] bounds = axes.getDisplayedBounds();

                double zDelta = (bounds[5] - bounds[4]) / 100;

                bounds[4] += zDelta * dy;
                bounds[5] += zDelta * dy;

                Boolean zoomed = tightZoomBoxToDataBounds(axes, bounds);
                boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
                ScaleUtils.applyInverseLogScaleToBounds(bounds, logFlags);

                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
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
