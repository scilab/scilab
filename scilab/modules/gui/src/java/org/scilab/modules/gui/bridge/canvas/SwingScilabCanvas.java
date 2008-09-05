/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.AWTEvent;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLJPanel;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.AxesRotationTracker;
import org.scilab.modules.gui.events.ScilabRubberBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;
import org.scilab.modules.renderer.utils.RenderingCapabilities;

import com.sun.opengl.util.Screenshot;

/**
 * Swing implementation for Scilab Canvas in GUIs This implementation requires
 * JOGL
 * 
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste silvy
 */
public class SwingScilabCanvas extends GLJPanel implements SimpleCanvas {

	private static final long serialVersionUID = 6101347094617535625L;

	/**
	 * Figure Index
	 * Used in Scilab to remember who is who.
	 */
	private int figureIndex;
	
	private GLEventListener renderer;
	
	private AxesRotationTracker rotationTracker;
	
	/**
	 * Constructor
	 * 
	 * @param cap
	 *            GLCapabilities associated to the GLJPanel
	 * @param figureIndex
	 *            index of the displayed figure
	 */
	public SwingScilabCanvas(GLCapabilities cap, int figureIndex) {
		super(cap);
		// TODO to remove, just for testing
		this.setLayout(null);
		renderer = new SciRenderer(figureIndex);
		this.addGLEventListener(renderer);
		this.figureIndex = figureIndex;
		
		// Focusable in order to catch KeyEvents...
		this.setFocusable(true);
		// Enable mouse Events sensitivity...
		this.enableEvents(AWTEvent.MOUSE_EVENT_MASK);
		
		rotationTracker = new AxesRotationTracker(this);
	}

	/**
	 * Create a Scilab Canvas
	 * 
	 * @param figureIndex index of the displayed figure
	 * @return the created canvas
	 */
	public static SwingScilabCanvas createCanvas(int figureIndex) {
		GLCapabilities cap = new GLCapabilities();
		
		SwingScilabCanvas newCanvas = new SwingScilabCanvas(cap, figureIndex);
		
		// I do this here and not in the ScilabCanvas because it is JOGL related stuff
		DrawableFigureGL correspondigFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		correspondigFigure.setRenderingTarget(newCanvas);
		
		return newCanvas;
	}

	/**
	 * Draws a Scilab canvas
	 * 
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * 
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getWidth(), this.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * 
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * 
	 * @param newSize
	 *            the size we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		// get the greatest size we can use
		int[] maxSize = RenderingCapabilities.getMaxCanvasSize();
		
		// make suze size is not greater than the max size
		Dimension finalDim = new Dimension(Math.min(newSize.getWidth(), maxSize[0]),
										   Math.min(newSize.getHeight(), maxSize[1]));
		
		setSize(finalDim);
		
		// if the size is too large, throw an exception
		if (newSize.getWidth() > maxSize[0] || newSize.getHeight() > maxSize[1]) {
			throw new IllegalArgumentException();
		}
		
	}
	

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * 
	 * @param newPosition
	 *            the position we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	/**
	 * Get the Figuer Index : the Scilab ID of the figure.
	 * 
	 * @return the ID.
	 */
	public int getFigureIndex() {
		return figureIndex;
	}
	
	/**
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * However JOGLSwingCanvas is always resized
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		
	}
	
	/**
	 * @return wether the resize mode is on or off
	 * However JOGLSwingCanvas is always resized
	 */
	public boolean getAutoResizeMode() {
		return true;
	}
	
	/**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
	public int[] getViewingRegion() {
		// here all the canvas is visible
		int[] res = {0, 0, getWidth(), getHeight()};
		return res;
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
		// nothing to do
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
	 * Set the event handler of the Canvas
	 * @param command the name of the Scilab function to call
	 */
	public void setEventHandler(String command) {
		// TODO Blouno !
	}

	/**
	 * Set the status of the event handler of the Canvas
	 * @param status is true to set the event handler active
	 */
	public void setEventHandlerEnabled(boolean status) {
		// TODO Blouno !
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
		return ScilabRubberBox.getRectangle(this, isClick, isZoom, initialRect, endRect);
	}
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the diplacement recording continue, false otherwise
	 */
	public boolean getRotationDisplacement(int[] displacement) {
		return rotationTracker.getDisplacement(displacement);
	}
	
	/**
	 * Ansynchrnous stop of rotation tracking.
	 */
	public void stopRotationRecording() {
		rotationTracker.cancelRecording();
	}
	
	/**
	 * Disable the canvas befor closing
	 */
	public void close() {
		// remove the event listener
		// so we won't have useless redraw
		
		removeGLEventListener(renderer);
		renderer = null;
		
		// context need to be destroyed
		// otherwise there are some memory leaks
		try {
			getContext().destroy();
		} catch (NullPointerException e) {
			// context not already created
			// do nothing
		}
		
		this.removeAll();
	}
	
	/**
	 * Take a screenshot of the figure and put it into a BufferedImage
	 * @return a BufferedImage
	 */
	public BufferedImage dumpAsBufferedImage() {			
		getContext().makeCurrent();		
		BufferedImage dump = Screenshot.readToBufferedImage(getWidth(), getHeight());		
		getContext().release();		

		return dump;
	}	
}	
