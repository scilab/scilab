/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL

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

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab Canvas in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabCanvas extends ScilabDockable implements Canvas {

    private SimpleCanvas component;

    /**
     * Constructor
     * @param figureIndex index of the displayed figure
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     */
    protected ScilabCanvas(int figureIndex, int antialiasingQuality) {
        component = ScilabBridge.createCanvas(figureIndex, antialiasingQuality);
    }

    /**
     * Creates a Scilab Canvas
     * @param figureIndex index of the displayed figure
     * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
     * @return the created canvas
     */
    public static Canvas createCanvas(int figureIndex, int antialiasingQuality) {
        return new ScilabCanvas(figureIndex, antialiasingQuality);
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleCanvas getAsSimpleCanvas() {
        return component;
    }

    /**
     * Sets a MenuBar to an element
     * @param newMenuBar the MenuBar to set to the element
     */
    public void addMenuBar(MenuBar newMenuBar) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException();
    }

    /**
     * Sets a ToolBar to an element
     * @param newToolBar the ToolBar to set to the element
     */
    public void addToolBar(ToolBar newToolBar) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException();
    }

    /**
     * Draws a Scilab canvas
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        ScilabBridge.draw(this);
    }

    /**
     * Gets the dimensions (width and height) of a Scilab Canvas
     * @return the size of the canvas
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        return ScilabBridge.getDims(this);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     * @return the position of the canvas
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        return ScilabBridge.getPosition(this);
    }

    /**
     * Gets the visibility status of a Scilab Canvas
     * @return the visibility status of the canvas (true if the canvas is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Sets the dimensions (width and height) of a Scilab Canvas
     * @param newSize the size we want to set to the canvas
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        ScilabBridge.setDims(this, newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
     * @param newPosition the position we want to set to the canvas
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        ScilabBridge.setPosition(this, newPosition);
    }

    /**
     * Sets the visibility status of a Scilab Canvas
     * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * Force the canvas to render itself immediately.
     */
    public void display() {
        ScilabBridge.display(this);
    }

    /**
      * Get the GL pipeline used by the canvas
      * @return GL pipeline
     */
    public GL getGL() {
        return ScilabBridge.getGL(this);
    }

    /**
     * @param onOrOff set whether the swap buffer mode is on or off.
     */
    public void setAutoSwapBufferMode(boolean onOrOff) {
        ScilabBridge.setAutoSwapBufferMode(this, onOrOff);
    }

    /**
     * @return set whether the swap buffer mode is on or off.
     */
    public boolean getAutoSwapBufferMode() {
        return ScilabBridge.getAutoSwapBufferMode(this);
    }

    /**
     * Force the canvas to render itself with synchronisation with its OpenGL process.
     */
    public void repaint() {
        ScilabBridge.repaint(this);
    }


    /**
     * Set the background of the Canvas.
     * @param red red channel
     * @param green green channel
     * @param blue blue channel
     */
    public void setBackgroundColor(double red, double green, double blue) {
        ScilabBridge.setBackgroundColor(this, red, green, blue);
    }

    /**
      * Disable the canvas before closing
      */
    public void close() {
        ScilabBridge.close(this);
    }

    /**
     * Convert to a BufferedImage
     * @return BufferedImage
     */
    public BufferedImage dumpAsBufferedImage() {
        return ScilabBridge.dumpAsBufferedImage(this);
    }

    /**
     * Set double buffer mode on or Off
     * @param useSingleBuffer if true use single buffer if false use double buffering
     */
    public void setSingleBuffered(boolean useSingleBuffer) {
        ScilabBridge.setSingleBuffered(this, useSingleBuffer);
    }

}
