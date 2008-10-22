/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.canvas;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import javax.media.opengl.GL;

import org.scilab.modules.gui.bridge.canvas.ScrolledSwingScilabCanvas;
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
	 * @return the created canvas
	 */
	public static SimpleCanvas createCanvas(int figureIndex) {
		return ScrolledSwingScilabCanvas.createCanvas(figureIndex);
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
	 * @param onOrOff set wether the swap buffer mode is on or off.
	 */
	public static void setAutoSwapBufferMode(Canvas canvas, boolean onOrOff) {
		canvas.getAsSimpleCanvas().setAutoSwapBufferMode(onOrOff);
	}
	
	/**
	 * @param canvas the canvas from which we want to retrieve the swap buffer mode.
	 * @return set wether the swap buffer mode is on or off.
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
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 * @param canvas the canvas we want to repaint
	 */
	public static void setAutoResizeMode(Canvas canvas, boolean onOrOff) {
		canvas.getAsSimpleCanvas().setAutoResizeMode(onOrOff);
	}
	
	/**
	 * @param canvas the canvas we want to repaint
	 * @return wether the resize mode is on or off
	 */
	public static boolean getAutoResizeMode(Canvas canvas) {
		return canvas.getAsSimpleCanvas().getAutoResizeMode();
	}
	
	/**
	 * Get the part of the canvas which is currently viewed
	 * @param canvas the canvas we want to repaint
	 * @return [x,y,w,h] array
	 */
	public static int[] getViewingRegion(Canvas canvas) {
		return canvas.getAsSimpleCanvas().getViewingRegion();
	}
	
	/**
	 * Specify a new viewport for the canvas
	 * For SwingScilabCanvas viewport can not be modified
	 * since it match the parent tab size
	 * @param canvas the canvas we want to modify
	 * @param posX X coordinate of upper left point of the viewport within the canvas
	 * @param posY Y coordinate of upper left point of the viewport within the canvas
	 * @param width width of the viewport
	 * @param height height of the viewport
	 */
	public static void setViewingRegion(Canvas canvas, int posX, int posY, int width, int height) {
		canvas.getAsSimpleCanvas().setViewingRegion(posX, posY, width, height);
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
	 * Set the event handler of the Canvas
	 * @param canvas the Canvas
	 * @param command the name of the Scilab function to call
	 */
	public static void setEventHandler(Canvas canvas, String command) {
		canvas.getAsSimpleCanvas().setEventHandler(command);
	}

	/**
	 * Set the status of the event handler of the Canvas
	 * @param canvas the Canvas
	 * @param status is true to set the event handler active
	 */
	public static void setEventHandlerEnabled(Canvas canvas, boolean status) {
		canvas.getAsSimpleCanvas().setEventHandlerEnabled(status);
	}
	
	/**
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param canvas canvas on which the rubber box will be applied
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public static int rubberBox(Canvas canvas, boolean isClick, boolean isZoom, int[] initialRect, int[] endRect) {
		return canvas.getAsSimpleCanvas().rubberBox(isClick, isZoom, initialRect, endRect);
	}
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param canvas canvas on which the displacement is recorded
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the diplacement recording continue, false otherwise
	 */
	public static boolean getRotationDisplacement(Canvas canvas, int[] displacement) {
		return canvas.getAsSimpleCanvas().getRotationDisplacement(displacement);
	}
	
	/**
	 * Ansynchrnous stop of rotation tracking.
	 * @param canvas canvas on which the displacement is be recorded
	 */
	public static void stopRotationRecording(Canvas canvas) {
		canvas.getAsSimpleCanvas().stopRotationRecording();
	}
	
	/**
	  * Disable the canvas befor closing
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
}
