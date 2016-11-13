/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
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

package org.scilab.modules.gui.canvas;

import java.awt.image.BufferedImage;

import javax.media.opengl.GL;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab Canvas in GUIs
 * @author Vincent COUVERT
 */
public class ScilabCanvasBridge {

    /**
     * Constructor
     */
    protected ScilabCanvasBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab Canvas
     * @param figureIndex index of the displayed figure
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     * @return the created canvas
     */
    public static SimpleCanvas createCanvas(int figureIndex, int antialiasingQuality) {
        return SwingScilabCanvas.createCanvas(figureIndex, antialiasingQuality);
    }

    /**
     * Draws a Scilab canvas
     * @param canvas the canvas to draw
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public static void draw(Canvas canvas) {
        canvas.getAsSimpleCanvas().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab Canvas
     * @param canvas the canvas we want to get the dimensions of
     * @return the size of the canvas
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public static Size getDims(Canvas canvas) {
        return canvas.getAsSimpleCanvas().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     * @param canvas the canvas we want to get the position of
     * @return the position of the canvas
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public static Position getPosition(Canvas canvas) {
        return canvas.getAsSimpleCanvas().getPosition();
    }

    /**
     * Gets the visibility status of a Scilab Canvas
     * @param canvas the canvas we want to get the visiblity status of
     * @return the visibility status of the canvas (true if the canvas is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public static boolean isVisible(Canvas canvas) {
        return canvas.getAsSimpleCanvas().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab Canvas
     * @param canvas the canvas we want to set the dimensions of
     * @param newSize the size we want to set to the canvas
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public static void setDims(Canvas canvas, Size newSize) {
        canvas.getAsSimpleCanvas().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     * @param canvas the canvas we want to set the position of
     * @param newPosition the position we want to set to the canvas
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public static void setPosition(Canvas canvas, Position newPosition) {
        canvas.getAsSimpleCanvas().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab Canvas
     * @param canvas the canvas we want to set the visiblity status of
     * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public static void setVisible(Canvas canvas, boolean newVisibleState) {
        canvas.getAsSimpleCanvas().setVisible(newVisibleState);
    }

    /**
     * @param canvas the canvas we want to display
     */
    public static void display(Canvas canvas) {
        canvas.getAsSimpleCanvas().display();
    }

    /**
     * @param canvas the canvas we want to get the GL object from
     * @return GL pipeline of the object
     */
    public static GL getGL(Canvas canvas) {
        return canvas.getAsSimpleCanvas().getGL();
    }

    /**
     * @param canvas the canvas of which we want to set the swap buffer mode.
     * @param onOrOff set whether the swap buffer mode is on or off.
     */
    public static void setAutoSwapBufferMode(Canvas canvas, boolean onOrOff) {
        canvas.getAsSimpleCanvas().setAutoSwapBufferMode(onOrOff);
    }

    /**
     * @param canvas the canvas from which we want to retrieve the swap buffer mode.
     * @return set whether the swap buffer mode is on or off.
     */
    public static boolean getAutoSwapBufferMode(Canvas canvas) {
        return canvas.getAsSimpleCanvas().getAutoSwapBufferMode();
    }

    /**
     * @param canvas the canvas we want to repaint
     */
    public static void repaint(Canvas canvas) {
        canvas.getAsSimpleCanvas().repaint();
    }

    /**
     * Set the background of the Canvas.
     * @param canvas the canvas we want to modify
     * @param red red channel
     * @param green green channel
     * @param blue blue channel
     */
    public static void setBackgroundColor(Canvas canvas, double red, double green, double blue) {
        canvas.getAsSimpleCanvas().setBackgroundColor(red, green, blue);
    }

    /**
      * Disable the canvas before closing
      * @param canvas canvas to close
      */
    public static void close(Canvas canvas) {
        canvas.getAsSimpleCanvas().close();
    }

    /**
     * Convert a ScilabCanvas to a BufferedImage
     * @param scilabCanvas ScilabCanvas
     * @return BufferedImage
     */
    public static BufferedImage dumpAsBufferedImage(ScilabCanvas scilabCanvas) {
        return scilabCanvas.getAsSimpleCanvas().dumpAsBufferedImage();
    }

    /**
     * Set double buffer mode on or Off
     * @param useSingleBuffer if true use single buffer if false use double buffering
     * @param canvas canvas to modify
     */
    public static void setSingleBuffered(Canvas canvas, boolean useSingleBuffer) {
        canvas.getAsSimpleCanvas().setSingleBuffered(useSingleBuffer);
    }
}
