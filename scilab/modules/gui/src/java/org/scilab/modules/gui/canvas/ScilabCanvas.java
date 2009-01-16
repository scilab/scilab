/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL

 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
	 */
	protected ScilabCanvas(int figureIndex) {
		component = ScilabBridge.createCanvas(figureIndex);
	}

	/**
	 * Creates a Scilab Canvas
	 * @param figureIndex index of the displayed figure
	 * @return the created canvas
	 */
	public static Canvas createCanvas(int figureIndex) {
		return new ScilabCanvas(figureIndex);
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
	 * @param onOrOff set wether the swap buffer mode is on or off.
	 */
	public void setAutoSwapBufferMode(boolean onOrOff) {
		ScilabBridge.setAutoSwapBufferMode(this, onOrOff);
	}
	
	/**
	 * @return set wether the swap buffer mode is on or off.
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
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param isClick specify whether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public int rubberBox(boolean isClick, boolean isZoom, int[] initialRect, int[] endRect) {
		return ScilabBridge.rubberBox(this, isClick, isZoom, initialRect, endRect);
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
