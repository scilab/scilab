/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
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
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.swing.JPanel;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.GlobalEventWatcher;
import org.scilab.modules.gui.graphicWindow.PanelLayout;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * Swing implementation for Scilab Canvas in GUIs This implementation requires
 * JOGL
 *
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabCanvas extends JPanel implements SimpleCanvas {

    private static final long serialVersionUID = 6101347094617535625L;

    /** The renderer canvas */
    private final Canvas rendererCanvas;

    /** The drawn figure */
    private AxesContainer figure;

    /** The drawer visitor used to draw the figure */
    private DrawerVisitor drawerVisitor;

    /** The drawable component where the draw is performed */
    private final Component drawableComponent;

    private Integer id;

    public SwingScilabCanvas(final AxesContainer figure) {
        super(new PanelLayout());
        this.figure = figure;

        drawableComponent = SwingScilabCanvasImpl.getInstance().createOpenGLComponent();

        drawableComponent.setEnabled(true);
        drawableComponent.setVisible(true);

        add(drawableComponent, PanelLayout.GL_CANVAS);

        rendererCanvas = JoGLCanvasFactory.createCanvas((GLAutoDrawable) drawableComponent);
        drawerVisitor = null;

        /* Workaround for bug 12682: setFocusable(false) did not work...
           the GLJPanel always got the focus after zooming. So when it gets it, the
           canvas will get it.*/
        drawableComponent.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                SwingScilabCanvas.this.requestFocus();
            }
        });

        drawerVisitor = new DrawerVisitor(drawableComponent, rendererCanvas, figure);
        rendererCanvas.setMainDrawer(drawerVisitor);
        drawableComponent.addMouseListener(new MouseAdapter() {
            public void mouseEntered(MouseEvent e) {
                GlobalEventWatcher.setAxesUID(figure.getIdentifier());
            }
        });
        setBackground(Color.white);
        setFocusable(true);
        setEnabled(true);
    }

    public void addNotify() {
        drawableComponent.setVisible(true);
        drawableComponent.setEnabled(true);
        add(drawableComponent, PanelLayout.GL_CANVAS);
        super.addNotify();
    }

    public void removeNotify() {//Thread.dumpStack();
        drawableComponent.setVisible(false);
        drawableComponent.setEnabled(false);
        remove(drawableComponent);

        super.removeNotify();
    }

    /**
     * Rendering canvas getter.
     * @return the SciRenderer canvas.
     */
    public Canvas getRendererCanvas() {
        return rendererCanvas;
    }

    /**
     * figure getter.
     * @return the MVC figure.
     */
    public AxesContainer getFigure() {
        return figure;
    }

    /**
     * Create a Scilab Canvas
     *
     * @param figureIndex index of the displayed figure
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     * @return the created canvas
     */
    public static SwingScilabCanvas createCanvas(int figureIndex, int antialiasingQuality) {
        return null;
    }

    /**
     * Draws a Scilab canvas
     *
     * @see org.scilab.modules.gui.canvas.SimpleCanvas#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab Canvas
     *
     * @return the size of the canvas
     * @see org.scilab.modules.gui.canvas.SimpleCanvas#getDims()
     */
    public Size getDims() {
        return new Size(this.getWidth(), this.getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     *
     * @return the position of the canvas
     * @see org.scilab.modules.gui.canvas.SimpleCanvas#getPosition()
     */
    public Position getPosition() {
        return new Position(this.getX(), this.getY());
    }

    /**
     * Sets the dimensions (width and height) of a Scilab Canvas
     *
     * @param newSize
     *            the size we want to set to the canvas
     * @see org.scilab.modules.gui.canvas.SimpleCanvas#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        // make suze size is not greater than the max size
        Dimension finalDim = new Dimension(newSize.getWidth(), newSize.getHeight());
        setSize(finalDim);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     *
     * @param newPosition
     *            the position we want to set to the canvas
     * @see org.scilab.modules.gui.canvas.SimpleCanvas#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        this.setLocation(newPosition.getX(), newPosition.getY());
    }
    /**
     * Get the Figure Index : the Scilab ID of the figure.
     *
     * @return the ID.
     */
    public int getFigureIndex() {
        // to avoid storing the data everywhere
        return getParentAxes().getFigureId();
    }


    /**
     * Set the background of the Canvas.
     * @param red red channel
     * @param green green channel
     * @param blue blue channel
     */
    public void setBackgroundColor(double red, double green, double blue) {
        this.setBackground(new Color((float) red, (float) green, (float) blue));
    }

    /**
     * Disable the canvas befor closing
     */
    public void close() {
    }

    /**
     * Take a screenshot of the figure and put it into a BufferedImage
     * @return a BufferedImage
     */
    public BufferedImage dumpAsBufferedImage() {
        Canvas canvas = getRendererCanvas();
        if (canvas instanceof JoGLCanvas) {
            return ((JoGLCanvas) canvas).getImage();
        }

        return null;
    }

    /**
     * Set double buffer mode on or Off
     * @param useSingleBuffer if true use single buffer if false use double buffering
     */
    public void setSingleBuffered(boolean useSingleBuffer) {

    }

    /**
     * @return the axes object containing the canvas
     */
    private SwingScilabAxes getParentAxes() {
        return null;
    }

    /**
     * Override set cursor in order to be able to modify the cursor
     * on the axes and not on the canvas itself
     * @param newCursor cursor to apply on the canvas
     */
    public void setCursor(Cursor newCursor) {
        setCursor(newCursor);
    }

    /**
     * The canvas is not focusable, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void addFocusListener(FocusListener listener) {
        //        getParentAxes().addFocusListener(listener);
    }

    /**
     * The canvas is not focusable, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void removeFocusListener(FocusListener listener) {
    }

    /**
     * The canvas is not enabled, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void addMouseListener(MouseListener listener) {
    }

    /**
     * The canvas is not enabled, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void removeMouseListener(MouseListener listener) {
    }

    /**
     * The canvas is not enabled, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void addMouseMotionListener(MouseMotionListener listener) {
    }

    /**
     * The canvas is not enabled, so add the listener to the parent instead
     * @param listener listener to add
     */
    public void removeMouseMotionListener(MouseMotionListener listener) {
    }

    /**
     * Adds the listener handling key events to the canvas.
     * @param listener listener to add.
     */
    public void addEventHandlerKeyListener(KeyListener listener) {
        addKeyListener(listener);
    }

    /**
     * Removes the listener handling key events from the canvas.
     * @param listener listener to remove.
     */
    public void removeEventHandlerKeyListener(KeyListener listener) {
        removeKeyListener(listener);
    }

    /**
     * Adds the listener handling mouse events to the canvas.
     * @param listener listener to add.
     */
    public void addEventHandlerMouseListener(MouseListener listener) {
        drawableComponent.addMouseListener(listener);
    }

    /**
     * Removes the listener handling mouse events from the canvas.
     * @param listener listener to remove.
     */
    public void removeEventHandlerMouseListener(MouseListener listener) {
        drawableComponent.removeMouseListener(listener);
    }

    /**
     * Adds the listener handling mouse motion events to the canvas.
     * @param listener listener to add.
     */
    public void addEventHandlerMouseMotionListener(MouseMotionListener listener) {
        drawableComponent.addMouseMotionListener(listener);
    }

    /**
     * Removes the listener handling mouse motion events from the canvas.
     * @param listener listener to remove.
     */
    public void removeEventHandlerMouseMotionListener(MouseMotionListener listener) {
        drawableComponent.removeMouseMotionListener(listener);
    }

    @Override
    public void display() {
        // TODO Auto-generated method stub
    }
    @Override
    public boolean getAutoSwapBufferMode() {
        // TODO Auto-generated method stub
        return false;
    }
    @Override
    public GL getGL() {
        // TODO Auto-generated method stub
        return null;
    }
    @Override
    public void setAutoSwapBufferMode(boolean onOrOff) {
        // TODO Auto-generated method stub
    }

    public boolean isAutoResize() {
        Boolean b = (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), __GO_AUTORESIZE__);
        return b == null ? false : b;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }
}
