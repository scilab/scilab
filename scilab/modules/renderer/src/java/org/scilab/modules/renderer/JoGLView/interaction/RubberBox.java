/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
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

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.text.DecimalFormat;

import java.util.ArrayList;

import javax.swing.event.EventListenerList;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.ruler.AxesRulerSpriteFactory;
import org.scilab.modules.renderer.JoGLView.interaction.util.HelpersGeometry;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointAComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointBComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointCComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointComputer;
import org.scilab.modules.renderer.JoGLView.interaction.util.PointDComputer;
import org.scilab.modules.renderer.JoGLView.postRendering.PostRendered;
import org.scilab.modules.localization.Messages;

/**
 * @author Pierre Lando
 */
public class RubberBox extends FigureInteraction implements PostRendered, MouseListener, MouseMotionListener, KeyListener {

    protected class AxesZoom {

        public final Axes axes;
        public PointComputer pointAComputer;
        public PointComputer pointBComputer;
        public PointComputer pointCComputer;
        public PointComputer pointDComputer;
        public Vector3d firstPoint;
        public Vector3d secondPoint;

        public AxesZoom(Axes axes) {
            this.axes = axes;
            firstPoint = new Vector3d(0.0, 0.0, 0.0);
            secondPoint = new Vector3d(0.0, 0.0, 0.0);
        }
    };

    /** Decimal format used to show info messages */
    private static final DecimalFormat DECIMAL_FORMAT = new DecimalFormat("0.###E0");

    static {
        AxesRulerSpriteFactory.setScilabStyle(DECIMAL_FORMAT);
    }

    /** Axes name used to show info messages */
    private static final String[] AXES_NAMES = new String[] {"X", "Y", "Z"};

    /** The cube indices */
    private static final int[] CUBE_INDICES = {
        0, 1, 3, 2, 4, 5, 7, 6,
        0, 3, 1, 2, 4, 7, 5, 6,
        0, 4, 1, 5, 3, 7, 2, 6
    };

    /** Rubber box status */
    public static enum Status {
        WAIT_POINT_A,
        WAIT_POINT_B,
        WAIT_POINT_C,
        WAIT_POINT_D,
    }

    /** Rubber box color */
    private static final Color RUBBER_BOX_COLOR = new Color(.2f, .3f, .4f);

    /** Rubber box thickness */
    private static final float RUBBER_BOX_THICKNESS = 2;

    /** Rubber box pattern */
    private static final short RUBBER_BOX_PATTERN = (short) 0xFAFA;

    /** Helpers appearance */
    private static Appearance helpersAppearance;

    /** Rubber box cube appearance */
    private static Appearance cubeAppearance;

    /** The event listener list */
    private final EventListenerList listenerList = new EventListenerList();

    /** Helpers geometry */
    private HelpersGeometry helpersGeometry;

    /** Rubber box cube geometry */
    private DefaultGeometry cubeGeometry;

    /** Current status */
    protected Status status;

    protected ArrayList<AxesZoom> axesZoomList;

    protected int mouseButton;

    protected Point lastPoint;

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    protected RubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        status = Status.WAIT_POINT_A;
        axesZoomList = new ArrayList<AxesZoom>();
        lastPoint = null;
    }

    /**
     * Add a rubber box listener.
     * The listener will be notified when a rubber box end.
     * @param rubberBoxListener the new listener.
     */
    public final void addListener(RubberBoxListener rubberBoxListener) {
        listenerList.add(RubberBoxListener.class, rubberBoxListener);
    }

    /**
     * Remove a rubber box listener.
     * The listener will no long be notified on events.
     * @param rubberBoxListener the removed listener.
     */
    public final void removeListener(RubberBoxListener rubberBoxListener) {
        listenerList.remove(RubberBoxListener.class, rubberBoxListener);
    }

    /**
     * Notify all listener that the rubber box have ended
     */
    protected void fireRubberBoxEnd() {
        for (RubberBoxListener rubberBoxListener : listenerList.getListeners(RubberBoxListener.class)) {
            rubberBoxListener.rubberBoxEnd();
        }
    }

    @Override
    public final void draw(DrawingTools drawingTools) throws SciRendererException {

        if (isEnable()) {
            for (AxesZoom axesZoom : axesZoomList) {
                drawingTools.getTransformationManager().useSceneCoordinate();
                drawingTools.getTransformationManager().getModelViewStack().push(
                    getDrawerVisitor().getAxesDrawer().getSceneProjection(axesZoom.axes.getIdentifier())
                );

                if (status != Status.WAIT_POINT_A) {
                    drawingTools.draw(getCubeGeometry(drawingTools, axesZoom), getCubeAppearance());
                }

                if (axesZoom.secondPoint != null) {
                    drawingTools.draw(getHelpersGeometry(drawingTools, axesZoom), getHelpersAppearance());
                }

                drawingTools.getTransformationManager().getModelViewStack().pop();
            }
        }
    }

    @Override
    public final void changeEnable(boolean isEnable) {
        Component component = getDrawerVisitor().getComponent();
        if (isEnable) {
            //status = Status.WAIT_POINT_A;
            axesZoomList.clear();
            component.addMouseListener(this);
            component.addMouseMotionListener(this);
            component.addKeyListener(this);
            component.setFocusTraversalKeysEnabled(false);
            component.setFocusable(true);
            component.requestFocus();
        } else {
            component.removeMouseListener(this);
            component.removeMouseMotionListener(this);
            component.removeKeyListener(this);
        }
        updateInfoMessage(null);
        getDrawerVisitor().getCanvas().redraw();
    }

    @Override
    public final void keyTyped(KeyEvent e) {
        if (e.getKeyChar() == KeyEvent.VK_ESCAPE) {
            mouseButton = -1;
            setEnable(false);
            fireRubberBoxEnd();
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        mouseButton = e.getButton();
        lastPoint = e.getPoint();
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
    public final void mouseMoved(MouseEvent e) {
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

    /**
     * Update displayed info message.
     */
    protected void updateInfoMessage(Point point) {
        if (isEnable()) {
            Axes axes = null;
            PointComputer pointAComputer = null, pointBComputer = null, pointCComputer = null, pointDComputer = null;
            AxesZoom axesZoom = getClosestAxesZoom(point);
            if (axesZoom != null) {
                axes = axesZoom.axes;
                pointAComputer = axesZoom.pointAComputer;
                pointBComputer = axesZoom.pointBComputer;
                pointCComputer = axesZoom.pointCComputer;
                pointDComputer = axesZoom.pointDComputer;
            }
            switch (status) {
                case WAIT_POINT_A:
                    setInfoMessage(Messages.gettext("Click to set first bounds"), axes, pointAComputer, false);
                    break;
                case WAIT_POINT_B:
                    setInfoMessage(Messages.gettext("Click to set second bounds"), axes, pointBComputer, false);
                    break;
                case WAIT_POINT_C:
                    setInfoMessage(Messages.gettext("Click to set first"), axes, pointCComputer, true);
                    break;
                case WAIT_POINT_D:
                    setInfoMessage(Messages.gettext("Click to set second"), axes, pointDComputer, true);
                    break;
                default:
            }
        } else {
            GraphicController.getController().setProperty(
                getDrawerVisitor().getFigure().getIdentifier(),
                GraphicObjectProperties.__GO_INFO_MESSAGE__,
                ""
            );
        }
    }

    /**
     * Set the info message.
     * @param baseMessage the base of the message.
     * @param axes the axes to get data from.
     * @param pointComputer current used point computer.
     * @param oneAxis true if only one coordinate is currently set.
     */
    private void setInfoMessage(String baseMessage, Axes axes, PointComputer pointComputer, boolean oneAxis) {
        if ((pointComputer != null) && (pointComputer.isValid())) {
            String message = baseMessage + " ";
            double[] data = pointComputer.getSecondPosition().getData();
            double[][] factors = axes.getScaleTranslateFactors();
            data[0] = (data[0] - factors[1][0]) / factors[0][0];
            data[1] = (data[1] - factors[1][1]) / factors[0][1];
            data[2] = (data[2] - factors[1][2]) / factors[0][2];

            String comma;
            if (oneAxis) {
                comma = "";
            } else {
                comma = ", ";
            }

            for (int i = 0; i < PointComputer.AXIS_NUMBER; i++) {
                if ((i != pointComputer.getFirstAxisIndex()) ^ oneAxis) {
                    message += AXES_NAMES[i] + " = " + DECIMAL_FORMAT.format(data[i]) + comma;
                    comma = "";
                }
            }
            GraphicController.getController().setProperty(
                getDrawerVisitor().getFigure().getIdentifier(),
                GraphicObjectProperties.__GO_INFO_MESSAGE__,
                message
            );
        } else {
            String message = Messages.gettext("Move your mouse on an axes box.");
            GraphicController.getController().setProperty(
                getDrawerVisitor().getFigure().getIdentifier(),
                GraphicObjectProperties.__GO_INFO_MESSAGE__,
                message
            );
        }
    }

    /**
     * Process action on RupperBox
     * Do nothing by default
     */
    protected void process() {
        // Do nothing
    }

    /**
     * Set the first point.
     * @param point first point AWT coordinate.
     * @return true if the first point is valid.
     */
    protected boolean setPointA(Point point) {
        axesZoomList.clear();
        boolean status = false;
        Axes[] allAxes = getAllVisibleAxes(point);
        for (Axes axes : allAxes) {
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

    /**
     * Set second point in 3D zoom.
     * @param point second point.
     * @return true if the point is valid.
     */
    protected boolean setPointB(Point point) {
        boolean status = false;
        for (AxesZoom axesZoom : axesZoomList) {
            PointBComputer pointComputer = new PointBComputer(axesZoom.axes, axesZoom.pointAComputer, point);
            if (pointComputer.isValid()) {
                axesZoom.pointBComputer = pointComputer;
                axesZoom.firstPoint = pointComputer.getFirstPosition();
                axesZoom.secondPoint = pointComputer.getSecondPosition();
                getDrawerVisitor().getCanvas().redraw();
                status = true;
            }
        }
        return status;
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    protected boolean setPointC(Point point) {
        boolean status = false;
        for (AxesZoom axesZoom : axesZoomList) {
            if (!axesZoom.pointBComputer.is2D()) {
                PointCComputer pointComputer = new PointCComputer(axesZoom.axes, axesZoom.pointBComputer, point);
                if (pointComputer.isValid()) {
                    axesZoom.pointCComputer = pointComputer;
                    axesZoom.firstPoint = pointComputer.getFirstPosition();
                    axesZoom.secondPoint = pointComputer.getSecondPosition();
                    getDrawerVisitor().getCanvas().redraw();
                    status = true;
                }
            }
        }
        return status;
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    protected boolean setPointD(Point point) {
        boolean status = false;
        for (AxesZoom axesZoom : axesZoomList) {
            if (!axesZoom.pointBComputer.is2D()) {
                PointDComputer pointComputer = new PointDComputer(axesZoom.axes, axesZoom.pointCComputer, point);
                if (pointComputer.isValid()) {
                    axesZoom.pointDComputer = pointComputer;
                    axesZoom.firstPoint = pointComputer.getFirstPosition();
                    axesZoom.secondPoint = pointComputer.getSecondPosition();
                    getDrawerVisitor().getCanvas().redraw();
                    status = true;
                }
            }
        }
        return status;
    }

    /**
     * Initialise or update the helpers geometry.
     * @param drawingTools the drawing tools used to draw the helpers.
     * @param axesZoom the structure containing the axes and points
     * @return updated helpers geometry.
     */
    private Geometry getHelpersGeometry(DrawingTools drawingTools, AxesZoom axesZoom) {
        if (helpersGeometry == null) {
            helpersGeometry = new HelpersGeometry(drawingTools);
        }
        helpersGeometry.updateVertex(axesZoom.axes, axesZoom.pointAComputer, axesZoom.secondPoint, status);
        return helpersGeometry;
    }

    /**
     * Helpers appearance getter.
     * First initialise the helpers appearance.
     * @return the helpers appearance.
     */
    public final Appearance getHelpersAppearance() {
        if (helpersAppearance == null) {
            helpersAppearance = new Appearance();
            helpersAppearance.setLineColor(new Color(1, 0, 0));
            helpersAppearance.setLineWidth(2);
        }
        return helpersAppearance;
    }

    /**
     * Rubber box cube geometry getter.
     * @param drawingTools the drawing tools.
     * @param axesZoom the structure containing the axes and points
     * @return the rubber box cubeGeometry.
     */
    private Geometry getCubeGeometry(DrawingTools drawingTools, AxesZoom axesZoom) {
        if (cubeGeometry == null) {
            cubeGeometry = new DefaultGeometry();

            BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
            ElementsBuffer vertexBuffer = bufferManager.createElementsBuffer();
            IndicesBuffer indicesBuffer = bufferManager.createIndicesBuffer();
            indicesBuffer.setData(CUBE_INDICES);

            cubeGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            cubeGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            cubeGeometry.setVertices(vertexBuffer);
            cubeGeometry.setWireIndices(indicesBuffer);
        }

        cubeGeometry.getVertices().setData(new float[] {
                                               (float) axesZoom.firstPoint.getX(), (float) axesZoom.firstPoint.getY(), (float) axesZoom.firstPoint.getZ(), 1,
                                               (float) axesZoom.firstPoint.getX(), (float) axesZoom.firstPoint.getY(), (float) axesZoom.secondPoint.getZ(), 1,
                                               (float) axesZoom.firstPoint.getX(), (float) axesZoom.secondPoint.getY(), (float) axesZoom.secondPoint.getZ(), 1,
                                               (float) axesZoom.firstPoint.getX(), (float) axesZoom.secondPoint.getY(), (float) axesZoom.firstPoint.getZ(), 1,
                                               (float) axesZoom.secondPoint.getX(), (float) axesZoom.firstPoint.getY(), (float) axesZoom.firstPoint.getZ(), 1,
                                               (float) axesZoom.secondPoint.getX(), (float) axesZoom.firstPoint.getY(), (float) axesZoom.secondPoint.getZ(), 1,
                                               (float) axesZoom.secondPoint.getX(), (float) axesZoom.secondPoint.getY(), (float) axesZoom.secondPoint.getZ(), 1,
                                               (float) axesZoom.secondPoint.getX(), (float) axesZoom.secondPoint.getY(), (float) axesZoom.firstPoint.getZ(), 1
                                           }, 4);

        return cubeGeometry;
    }

    /**
     * Rubber-box cube appearance getter.
     * @return the rubber-box cube appearance.
     */
    private Appearance getCubeAppearance() {
        if (cubeAppearance == null) {
            cubeAppearance = new Appearance();
            cubeAppearance.setLineColor(RUBBER_BOX_COLOR);
            cubeAppearance.setLineWidth(RUBBER_BOX_THICKNESS);
            cubeAppearance.setLinePattern(RUBBER_BOX_PATTERN);
        }
        return cubeAppearance;
    }

    /**
     * Calculates the minimum squared distance from
     * the given x,y to the given axes bounds.
     *
     * @param bounds the axes bounds [x, y, w, h]
     * @param x the coordinate x
     * @param y the coordinate y
     * @return the squared distance
     **/
    private double calculateAxesDistance2(Double[] bounds, double x, double y) {

        double x2 = bounds[0] + bounds[2];
        double y2 = bounds[1] + bounds[3];
        double x1 = bounds[0];
        double y1 = bounds[1];
        boolean xbounded = (x >= x1) && (x <= x2);
        boolean ybounded = (y >= y1) && (y <= y2);

        if (xbounded && ybounded) {
            return 0.0;
        }

        y1 -= y;
        y2 -= y;
        y1 *= y1;
        y2 *= y2;
        if (xbounded) {
            return Math.min(y1, y2);
        }

        x1 -= x;
        x2 -= x;
        x1 *= x1;
        x2 *= x2;
        if (ybounded) {
            return Math.min(x1, x2);
        }

        return Math.min(Math.min(x1 + y1, x1 + y2), Math.min(x2 + y1, x2 + y2));
    }

    /**
     * Selects the axesZoom structure containing
     * the closest axes.
     *
     * @param point the point to calculate distance
     * @return the axesZoom structure
     **/
    protected AxesZoom getClosestAxesZoom(Point point) {
        AxesZoom ret = null;
        if (point == null) {
            point = new Point(0, 0);
        }
        double minDist2 = Double.MAX_VALUE;
        Dimension size = drawerVisitor.getCanvas().getDimension();
        double x = point.getX() / size.getWidth();
        double y = point.getY() / size.getHeight();
        for (AxesZoom axesZoom : axesZoomList) {

            Double[] bounds = axesZoom.axes.getAxesBounds();//x, y, w, h
            double d2 = calculateAxesDistance2(bounds, x, y);
            if (minDist2 > d2) {
                ret = axesZoom;
                minDist2 = d2;
            }
        }

        return ret;
    }

    /**
     * Check if exists any valid 3d zoom
     * going on the axesZoomList.
     *
     * @return true if exists false otherwise
     **/
    protected boolean valid3D() {
        boolean valid = false;
        for (AxesZoom axesZoom : axesZoomList) {
            if (axesZoom.pointBComputer != null && !axesZoom.pointBComputer.is2D()) {
                Double[] bounds = { axesZoom.firstPoint.getX(), axesZoom.secondPoint.getX(),
                                    axesZoom.firstPoint.getY(), axesZoom.secondPoint.getY(),
                                    axesZoom.firstPoint.getZ(), axesZoom.secondPoint.getZ(),
                                  };
                int count = 0;
                count = bounds[0].compareTo(bounds[1]) != 0 ? ++count : count;
                count = bounds[2].compareTo(bounds[3]) != 0 ? ++count : count;
                count = bounds[4].compareTo(bounds[5]) != 0 ? ++count : count;
                //Atleast 2 axis must be different to be a valid zoom selection
                if (count >= 2) {
                    valid = true;
                }
            }
        }
        return valid;
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void mouseDragged(MouseEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }

    /**
     * Computes the current selection box
     * using the closest axes coordinates
     *
     * @return the selection box
     **/
    public double[] getResults() {
        AxesZoom axesZoom = getClosestAxesZoom(lastPoint);
        if (axesZoom != null) {
            double[][] factors = axesZoom.axes.getScaleTranslateFactors();
            double result[] = {
                mouseButton - 1,
                (Math.min(axesZoom.firstPoint.getX(), axesZoom.secondPoint.getX()) - factors[1][0]) / factors[0][0],
                (Math.max(axesZoom.firstPoint.getY(), axesZoom.secondPoint.getY()) - factors[1][1]) / factors[0][1],
                (Math.max(axesZoom.firstPoint.getZ(), axesZoom.secondPoint.getZ()) - factors[1][2]) / factors[0][2],
                (Math.abs(axesZoom.firstPoint.getX() - axesZoom.secondPoint.getX())) / factors[0][0],
                (Math.abs(axesZoom.firstPoint.getY() - axesZoom.secondPoint.getY())) / factors[0][1],
                (Math.abs(axesZoom.firstPoint.getZ() - axesZoom.secondPoint.getZ())) / factors[0][2]
            };
            return result;
        } else {
            return new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        }
    }
}
