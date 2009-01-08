/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
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
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.FocusListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.ScilabRubberBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;
import org.scilab.modules.renderer.utils.RenderingCapabilities;

import com.sun.opengl.util.Screenshot;

import javax.swing.SwingUtilities;
import java.lang.reflect.InvocationTargetException;

/**
 * Swing implementation for Scilab Canvas in GUIs This implementation requires
 * JOGL
 * 
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabCanvas extends SwingScilabCanvasImpl implements SimpleCanvas {

	private static final long serialVersionUID = 6101347094617535625L;

	private GLEventListener renderer;
	
	
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
		
		// create the GLEventListener
		renderer = new SciRenderer(figureIndex);
		this.addGLEventListener(renderer);
		
		// to avoid focusing on canvas
		setFocusable(false);
		
		// to avoid mouse events on canvas
		//setEnabled(false);
		
		
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
		FigureMapper.getCorrespondingFigure(figureIndex).setRenderingTarget(newCanvas);
		
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
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param isClick specify whether the rubber box is selected by one click for each one of the two edge
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
	 * Disable the canvas befor closing
	 */
	public void close() {
		// remove the event listener
		// so we won't have useless redraw
		
		removeGLEventListener(renderer);
		renderer = null;
		
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					// context need to be destroyed
					// otherwise there are some memory leaks
					getContext().destroy();
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			// if the context is not created, the context destruction
			// will raise a NullPointerException
			if (!(e.getCause() instanceof NullPointerException)) {
				// throw again the exception
				throw (GLException) e.getCause();
			}
		}
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
	
	/**
	 * Set double buffer mode on or Off
	 * @param useSingleBuffer if true use single buffer if false use double buffering
	 */
	public void setSingleBuffered(boolean useSingleBuffer) {
		// When in single buffer
		// we need to be sure that no incoming modifications will occur on the canvas
		// such as resize, needed repaint, etc...
		// Otherwise it might mess up the draw, specially with scicos.
		// So we wait until the event queue is totally empty.
		if (useSingleBuffer && getChosenGLCapabilities().getDoubleBuffered()) {
    		Object lock = new Object();
    		// Check if there are still events on the queue
    		while(Toolkit.getDefaultToolkit().getSystemEventQueue().peekEvent() != null) {
    			// if yes, wait a little to avoid consuming CPU.
    			synchronized (lock) {
					try {
						lock.wait(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
    		}
  	
    	}
		// nothing to do when switching back to double buffer
		// or if already in single buffer mode
	}
	
	/**
	 * @return the axes object containing the canvas
	 */
	private SwingScilabAxes getParentAxes() {
		return (SwingScilabAxes) getAsComponent().getParent();
	}
	
	/**
	 * Override set cursor in order to be able to modify the cursor
	 * on the axes and not on the canvas itself
	 * @param newCursor cursor to apply on the canvas
	 */
	public void setCursor(Cursor newCursor) {
		getParentAxes().setCursor(newCursor);
	}
	
	/**
	 * The canvas is not focusable, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addFocusListener(FocusListener listener) {
		getParentAxes().addFocusListener(listener);
	}
	
	/**
	 * The canvas is not focusable, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeFocusListener(FocusListener listener) {
		getParentAxes().removeFocusListener(listener);
	}
	
	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addMouseListener(MouseListener listener) {
		getParentAxes().addMouseListener(listener);
	}
	
	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeMouseListener(MouseListener listener) {
		getParentAxes().removeMouseListener(listener);
	}
	
	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addMouseMotionListener(MouseMotionListener listener) {
		getParentAxes().addMouseMotionListener(listener);
	}
	
	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeMouseMotionListener(MouseMotionListener listener) {
		getParentAxes().removeMouseMotionListener(listener);
	}
	
	

}	
