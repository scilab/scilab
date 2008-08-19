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
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		ScilabBridge.setAutoResizeMode(this, onOrOff);
	}

	/**
	 * @return wether the resize mode is on or off
	 */
	public boolean getAutoResizeMode() {
		return ScilabBridge.getAutoResizeMode(this);
	}
	
	/**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
	public int[] getViewingRegion() {
		return ScilabBridge.getViewingRegion(this);
	}
	
	/**
	 * Specify a new viewport for the canvas
	 * For SwingScilabCanvas viewport can not be modified
	 * since it match the parent tab size
	 * @param posX X coordinate of upper left point of the viewport within the canvas
	 * @param posY Y coordinate of upper left point of the viewport within the canvas
	 * @param width width of the viewport
	 * @param height height of the viewport
	 */
	public void setViewingRegion(int posX, int posY, int width, int height) {
		ScilabBridge.setViewingRegion(this, posX, posY, width, height);
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
	 * Set the event handler of the Canvas
	 * @param command the name of the Scilab function to call
	 */
	public void setEventHandler(String command) {
		ScilabBridge.setEventHandler(this, command);
	}

	/**
	 * Set the status of the event handler of the Canvas
	 * @param status is true to set the event handler active
	 */
	public void setEventHandlerEnabled(boolean status) {
		ScilabBridge.setEventHandlerEnabled(this, status);
	}
	
	/**
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
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
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the diplacement recording continue, false otherwise
	 */
	public boolean getRotationDisplacement(int[] displacement) {
		return ScilabBridge.getRotationDisplacement(this, displacement);
	}
	
	/**
	 * Ansynchrnous stop of rotation tracking.
	 */
	public void stopRotationRecording() {
		ScilabBridge.stopRotationRecording(this);
	}
	
	/**
	  * Disable the canvas befor closing
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
	
}
