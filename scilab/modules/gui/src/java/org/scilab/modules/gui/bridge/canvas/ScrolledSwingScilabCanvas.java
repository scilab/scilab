/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.Color;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.lang.reflect.InvocationTargetException;

import javax.media.opengl.GL;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Canvas without scrollbars in GUIs This implementation requires
 * JOGL
 *
 * @author Jean-Baptiste Silvy
 */
public class ScrolledSwingScilabCanvas extends JScrollPane implements SimpleCanvas {

	private static final int VIEWPORT_SIZE = 4;

	/** The real canvas */
	private ScrollabeSwingScilabCanvas canvas;

	private ScilabEventListener eventHandler;
	/**
	 * Default contsructor
	 * @param canvas the canvas to view
	 */
	public ScrolledSwingScilabCanvas(ScrollabeSwingScilabCanvas canvas) {
		super(canvas);
		this.canvas = canvas;
		// actually with the current impelementation of GLJPanel.paintComponent,
		// the panel is totally redrawed each time.
		this.getViewport().setScrollMode(JViewport.BLIT_SCROLL_MODE);
	}

	/**
	 * @param figureIndex index of the figure which will be drawn
	 * @return new instance of canvas ready to display the figure
	 */
	public static ScrolledSwingScilabCanvas createCanvas(int figureIndex) {
		ScrollabeSwingScilabCanvas viewedCanvas = ScrollabeSwingScilabCanvas.createCanvas(figureIndex);
		ScrolledSwingScilabCanvas newCanvas = new ScrolledSwingScilabCanvas(viewedCanvas);
		newCanvas.setBackgroundColor(1.0, 1.0, 1.0);
		return newCanvas;
	}

	/**
	 * Set the canvas implementation of the canvas
	 * @param canvas canvas to surround with scroll pane
	 */
	public void setCanvas(ScrollabeSwingScilabCanvas canvas) {
		this.canvas = canvas;
		this.setViewportView(canvas);
	}

	/**
	 * @return enclosed canvas
	 */
	protected ScrollabeSwingScilabCanvas getCanvas() {
		return canvas;
	}

	/**
	 * OpenGL function forcing redraw of the canvas
	 */
	public void display() {
		//long deb = System.nanoTime();
		getCanvas().display();
		//long end = System.nanoTime() - deb;
		//System.err.println("Elapsed time = " + (end / 1000000.0));
	}

	/**
	 * Scilab function to draw on object
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
		canvas.draw();
	}

	/**
	 * @return true if auto swap buffer mode is on.
	 */
	public boolean getAutoSwapBufferMode() {
		return getCanvas().getAutoSwapBufferMode();
	}

	/**
	 * @return size of the canvas in pixels
	 */
	public Size getDims() {
		return getCanvas().getDims();
	}

	/**
	 * @return current OpenGL pipeline used by this object
	 */
	public GL getGL() {
		return getCanvas().getGL();
	}

	/**
	 * @return get the position of the canvas within the
	 */
	public Position getPosition() {
		return new Position(getCanvas().getX(), getCanvas().getY());
	}

	/**
	 * Set the swap buffer mode
	 * @param onOrOff true to enable, false to disable
	 */
	public void setAutoSwapBufferMode(boolean onOrOff) {
		getCanvas().setAutoSwapBufferMode(onOrOff);

	}

	/**
	 * Set new size for the canvas
	 * @param newSize new Size to apply
	 */
	public void setDims(Size newSize) {
		getCanvas().setDims(newSize);
	}

	/**
	 * Set a new position for the canvas
	 * @param newPosition new position to apply
	 */
	public void setPosition(Position newPosition) {
		getCanvas().setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Get the Figuer Index : the Scilab ID of the figure.
	 *
	 * @return the ID.
	 */
	public int getFigureIndex() {
		return getCanvas().getFigureIndex();
	}

	/**
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		getCanvas().setAutoResizeMode(onOrOff);

		// so the canvas will retrieve its good position
		getCanvas().revalidate();
	}

	/**
	 * @return wether the resize mode is on or off
	 */
	public boolean getAutoResizeMode() {
		return getCanvas().getAutoResizeMode();
	}

	/**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
	public int[] getViewingRegion() {
		Rectangle viewport = getViewport().getViewRect();
		int[] res = {(int) viewport.getX(),
				(int) viewport.getY(),
				(int) viewport.getWidth(),
				(int) viewport.getHeight()};
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
		if (!getAutoResizeMode()) {
			// don't set viewport size here it should always fit parent tab size
			// It seems that we must check the viewport size and positions
			// to get coherent values, otherwise the setViewPosition hangs...
			// there are three checks that must be performed for the two dimensions
			// - be sure that viewport position is greater than 0.
			// - if the viewport is larger than the canvas, then it can't be moved
			// - if the viewport is smaller than the canvas, then it should remains
			//   inside the canvas

			int canvasWidth = getCanvas().getWidth();
			int canvasHeight = getCanvas().getHeight();
			int[] curViewedRegion = getViewingRegion();
			int viewportPosX = curViewedRegion[0];
			int viewPortPosY = curViewedRegion[1];
			int viewportWidth = curViewedRegion[2];
			int viewportHeight = curViewedRegion[VIEWPORT_SIZE - 1];

			// use previous values as default ones
			int realPosX = 0;
			int realPosY = 0;


			if (viewportWidth <= canvasWidth) {
				// viewport smaller than the canvas
				// check that the viewport stays in the canvas
				// the left most position is canvasWidth - viewporwidth
				realPosX = Math.min(posX, canvasWidth - viewportWidth);
			} else {
				// viewport larger than the canvas
				// get previous position (should be 0)
				realPosX = viewportPosX;
			}
			// last check, greater than 0
			realPosX = Math.max(0, realPosX);

			if (viewportHeight <= canvasHeight) {
				realPosY = Math.min(posY, canvasHeight - viewportHeight);
			} else {
				realPosY = viewPortPosY;
			}
			realPosY = Math.max(0, realPosY);

			getViewport().setViewPosition(new Point(realPosX, realPosY));
			revalidate();
		}
	}

	/**
	 * Set the background of the Canvas.
	 * @param red red channel
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setBackgroundColor(double red, double green, double blue) {
		getCanvas().setBackgroundColor(red, green, blue);
		this.setBackground(new Color((float) red, (float) green, (float) blue));
	}

	/**
	 * Set the event handler of the Canvas
	 * @param command the name of the Scilab function to call
	 */
	public void setEventHandler(String funName) {
		disableEventHandler();
		eventHandler = new ScilabEventListener(funName, getFigureIndex());
	}


	/**
	 * Set the status of the event handler of the Canvas
	 * @param status is true to set the event handler active
	 */
	public void setEventHandlerEnabled(boolean status) {
		if (status) {
			enableEventHandler();
		}
		else {
			disableEventHandler();
		}
	}

	private void enableEventHandler() {
		canvas.addKeyListener(eventHandler);
		canvas.addMouseListener(eventHandler);
		canvas.addMouseMotionListener(eventHandler);
	}

	private void disableEventHandler() {
		if (eventHandler != null) {
			canvas.removeKeyListener(eventHandler);
			canvas.removeMouseListener(eventHandler);
			canvas.removeMouseMotionListener(eventHandler);
		}
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
		return getCanvas().rubberBox(isClick, isZoom, initialRect, endRect);
	}
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the diplacement recording continue, false otherwise
	 */
	public boolean getRotationDisplacement(int[] displacement) {
		return getCanvas().getRotationDisplacement(displacement);
	}
	
	/**
	 * Ansynchrnous stop of rotation tracking.
	 */
	public void stopRotationRecording() {
		getCanvas().stopRotationRecording();
	}
	
	/**
	 * Disable the canvas befor closing
	 */
	public void close() {
		// need to call it on the event dispatch thread, otherwise setCanvas(null)
		// freezes gui.
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					disableEventHandler();
					getCanvas().close();
					setCanvas(null);
					removeAll();
					eventHandler = null;
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.getCause().printStackTrace();
		}
	}
	
	/**
	 * Convert a Canvas to an BufferedImage
	 * @return a BufferedImage
	 */
	public BufferedImage dumpAsBufferedImage() {
		return canvas.dumpAsBufferedImage();
	}
}
