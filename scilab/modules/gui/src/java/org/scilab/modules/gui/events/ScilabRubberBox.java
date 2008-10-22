/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.events;

import java.awt.event.HierarchyEvent;
import java.awt.event.HierarchyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.renderer.utils.selection.RubberBox;

/**
 * Class used to interactively get and display rectangular selection area.
 * @author Jean-Baptiste Silvy
 */
public abstract class ScilabRubberBox extends RubberBox
	implements MouseListener, MouseMotionListener, HierarchyListener {
	
	private static final int INITIAL_RECT_SIZE = 4;
	
	private static final int CLOSE_ACTION_BUTTON = -100;

	private boolean drawingMode;
	
	private SwingScilabCanvas selectedCanvas;
	
	private Object lock = new Object();
	
	private int terminateButton;
	
	/**
	 * Default constructor
	 * @param selectedCanvas Canvas on which the rubberbox will be added
	 */
	public ScilabRubberBox(SwingScilabCanvas selectedCanvas) {
		super();
		drawingMode = false;
		this.selectedCanvas = selectedCanvas;
		terminateButton = 0;
	}
	
	/**
	 * @return true if we are currently dragging the rectangle
	 */
	protected boolean isDragging() {
		return drawingMode;
	}
	
	/**
	 * @return Canvas on which the rubberbox is performed
	 */
	protected SwingScilabCanvas getSelectedCanvas() {
		return selectedCanvas;
	}
	
	/**
	 * Specify the Scilab code of the mouse button used to terminate
	 * the rubber box
	 * @param buttonCode Scilab code of the button
	 */
	protected void setUsedButton(int buttonCode) {
		terminateButton = buttonCode;
	}
	
	/**
	 * @return Scilab code of the button used to terminate the rubber box
	 */
	protected int getUsedButton() {
		return terminateButton;
	}
	
	/**
	 * Call rubber box on a canvas with the right rubber box
	 * @param canvas canvas on which the rubber box will be used.
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public static int getRectangle(SwingScilabCanvas canvas, boolean isClick, boolean isZoom,
								   int[] initialRect, int[] endRect) {
		ScilabRubberBox rubberBox = null;
		
		// select the kind of listener accordingly
		if (isZoom) {
			rubberBox = new ZoomRubberBox(canvas);
		} else {
			if (isClick) {
				rubberBox = new ClickRubberBox(canvas);
			} else {
				rubberBox = new PressReleaseRubberBox(canvas);
			}
		}
		
		
		return rubberBox.getRectangle(initialRect, endRect);
	}
	
	/**
	 * Call the rubber box on the canvas
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public int getRectangle(int[] initialRect, int[] endRect) {
		
		// for know we just track mouse button events
		// until we begin to dragg the rectangle
		selectedCanvas.addMouseListener(this);
		selectedCanvas.addHierarchyListener(this);
		
		if (initialRect != null) {
			// don't wait any click to start getting rectangle
			beginDragging(initialRect[0], initialRect[1], initialRect[2], initialRect[INITIAL_RECT_SIZE - 1]);
		}
		
		// wait until rubber box if finished
		synchronized (lock) {
			try {
				lock.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		selectedCanvas.removeMouseListener(this);
		selectedCanvas.removeHierarchyListener(this);
		
		endRect[0] = getFirstPointX();
		endRect[1] = getFirstPointY();
		endRect[2] = getSecondPointX();
		endRect[INITIAL_RECT_SIZE - 1] = getSecondPointY();

		return getUsedButton();
		
	}

	/**
	 * @param event event when the mouse enter the canvas
	 */
	public void mouseEntered(MouseEvent event) {
		// nothing to do
	}

	/**
	 * @param event event when the mouse exit the canvas
	 */
	public void mouseExited(MouseEvent event) {
		// nothing to do
	}


	/**
	 * Called when the mouse move with a button pressed
	 * @param event associated event
	 */
	public void mouseDragged(MouseEvent event) {
		// we just need to know that the mouse has been moved
		// mouseDragged and mouseMoved are exclusive
		mouseMoved(event);
	}

	/**
	 * Called when the mouse move with a button pressed
	 * @param event associated event
	 */
	public void mouseMoved(MouseEvent event) {
		if (isDragging()) {
			// just move second point an draw
			setSecondPoint(event.getX(), event.getY());
			draw();
		}
	}
	
	/**
	 * Begin the display and dragging of the selection rectangle
	 * @param firstPointX initial X coordinate for the first corner of the selection rectangle
	 * @param firstPointY initial Y coordinate for the first corner of the selection rectangle
	 * @param secondPointX initial X coordinate for the second corner of the selection rectangle
	 * @param secondPointY initial Y coordinate for the second corner of the selection rectangle
	 */
	protected void beginDragging(int firstPointX, int firstPointY, int secondPointX, int secondPointY) {
		
		// set initial dragging rectangle coordinates
		setFirstPoint(firstPointX, firstPointY);
		setSecondPoint(secondPointX, secondPointY);
		
		// activate the canvas
		activate(selectedCanvas.getFigureIndex());
		
		// now track mouse motion event to update the rectangle position
		selectedCanvas.addMouseMotionListener(this);
		
		drawingMode = true;
		draw();
	}
	
	/**
	 * End the display of the dragging rectangle
	 */
	protected void endDragging() {
		// desactivate drawing of rectangle
		desactivate();
		
		// remove listener from the canvas
		selectedCanvas.removeMouseMotionListener(this);
		
		// wake up calling thread
		synchronized (lock) {
			lock.notifyAll();
		}
	}
	
	/**
	 * Event called when hierarchy changed. It is for example called when the
	 * canvas is destroyed.
	 * @param event hierarchy event
	 */
	public void hierarchyChanged(HierarchyEvent event) {
		// we should stop recording here
		if (isDragging()) {
			endDragging();
		}
		selectedCanvas.removeMouseListener(this);
		selectedCanvas.removeHierarchyListener(this);
		/* specify that canvas has been closed*/
		setUsedButton(CLOSE_ACTION_BUTTON);
		synchronized (lock) {
			lock.notifyAll();
		}
	}

}
