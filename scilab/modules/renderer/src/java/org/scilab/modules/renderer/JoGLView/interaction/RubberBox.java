/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import java.awt.Component;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.text.DecimalFormat;

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

    protected Axes axes;

    private PointComputer pointAComputer;
    protected PointComputer pointBComputer;
    private PointComputer pointCComputer;
    private PointComputer pointDComputer;
    protected Vector3d firstPoint;
    protected Vector3d secondPoint;

    protected int mouseButton;

    /**
     * Default constructor.
     *
     * @param drawerVisitor parent drawer visitor.
     */
    protected RubberBox(DrawerVisitor drawerVisitor) {
        super(drawerVisitor);
        status = Status.WAIT_POINT_A;
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
        if (isEnable() && (axes != null)) {
            drawingTools.getTransformationManager().useSceneCoordinate();
            drawingTools.getTransformationManager().getModelViewStack().push(
                getDrawerVisitor().getAxesDrawer().getSceneProjection(axes.getIdentifier())
            );

            if (status != Status.WAIT_POINT_A) {
                drawingTools.draw(getCubeGeometry(drawingTools), getCubeAppearance());
            }

            if (secondPoint != null) {
                drawingTools.draw(getHelpersGeometry(drawingTools), getHelpersAppearance());
            }

            drawingTools.getTransformationManager().getModelViewStack().pop();
        }
    }

    @Override
    public final void changeEnable(boolean isEnable) {
        Component component = getDrawerVisitor().getComponent();
        if (isEnable) {
            //status = Status.WAIT_POINT_A;
            pointAComputer = null;
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
        updateInfoMessage();
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
        updateInfoMessage();
    }

    /**
     * Update displayed info message.
     */
    protected void updateInfoMessage() {
        if (isEnable()) {
            switch (status) {
                case WAIT_POINT_A:
                    setInfoMessage(Messages.gettext("Click to set first bounds"), pointAComputer, false);
                    break;
                case WAIT_POINT_B:
                    setInfoMessage(Messages.gettext("Click to set second bounds"), pointBComputer, false);
                    break;
                case WAIT_POINT_C:
                    setInfoMessage(Messages.gettext("Click to set first"), pointCComputer, true);
                    break;
                case WAIT_POINT_D:
                    setInfoMessage(Messages.gettext("Click to set second"), pointDComputer, true);
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
     * @param pointComputer current used point computer.
     * @param oneAxis true if only one coordinate is currently set.
     */
    private void setInfoMessage(String baseMessage, PointComputer pointComputer, boolean oneAxis) {
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
        axes = getUnderlyingAxes(point);
        if (axes != null) {
            PointAComputer pointComputer = new PointAComputer(axes, point);
            if (pointComputer.isValid()) {
                this.pointAComputer = pointComputer;
                firstPoint = pointComputer.getPosition();
                secondPoint = firstPoint;
                return true;
            }
        }
        return false;
    }

    /**
     * Set second point in 3D zoom.
     * @param point second point.
     * @return true if the point is valid.
     */
    protected boolean setPointB(Point point) {
        PointBComputer pointComputer = new PointBComputer(axes, pointAComputer, point);
        if (pointComputer.isValid()) {
            this.pointBComputer = pointComputer;
            firstPoint = pointComputer.getFirstPosition();
            secondPoint = pointComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    protected boolean setPointC(Point point) {
        PointCComputer pointComputer = new PointCComputer(axes, pointBComputer, point);
        if (pointComputer.isValid()) {
            this.pointCComputer = pointComputer;
            firstPoint = pointComputer.getFirstPosition();
            secondPoint = pointComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Set zoom box position in 3D zoom.
     * @param point mouse position.
     * @return true if the point is valid.
     */
    protected boolean setPointD(Point point) {
        PointDComputer pointComputer = new PointDComputer(axes, pointCComputer, point);
        if (pointComputer.isValid()) {
            this.pointDComputer = pointComputer;
            firstPoint = pointComputer.getFirstPosition();
            secondPoint = pointComputer.getSecondPosition();
            getDrawerVisitor().getCanvas().redraw();
            return true;
        } else {
            return false;
        }
    }

    /**
     * Initialise or update the helpers geometry.
     * @param drawingTools the drawing tools used to draw the helpers.
     * @return updated helpers geometry.
     */
    private Geometry getHelpersGeometry(DrawingTools drawingTools) {
        if (helpersGeometry == null) {
            helpersGeometry = new HelpersGeometry(drawingTools);
        }
        helpersGeometry.updateVertex(axes, pointAComputer, secondPoint, status);
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
     * @return the rubber box cubeGeometry.
     */
    private Geometry getCubeGeometry(DrawingTools drawingTools) {
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
                                               (float) firstPoint.getX(), (float) firstPoint.getY(), (float) firstPoint.getZ(), 1,
                                               (float) firstPoint.getX(), (float) firstPoint.getY(), (float) secondPoint.getZ(), 1,
                                               (float) firstPoint.getX(), (float) secondPoint.getY(), (float) secondPoint.getZ(), 1,
                                               (float) firstPoint.getX(), (float) secondPoint.getY(), (float) firstPoint.getZ(), 1,
                                               (float) secondPoint.getX(), (float) firstPoint.getY(), (float) firstPoint.getZ(), 1,
                                               (float) secondPoint.getX(), (float) firstPoint.getY(), (float) secondPoint.getZ(), 1,
                                               (float) secondPoint.getX(), (float) secondPoint.getY(), (float) secondPoint.getZ(), 1,
                                               (float) secondPoint.getX(), (float) secondPoint.getY(), (float) firstPoint.getZ(), 1
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
}
