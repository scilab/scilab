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

import java.awt.Component;
import java.awt.Cursor;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import org.scilab.modules.gui.utils.ScilabSwingUtilities;


/**
 * Class used to retrieve displacement that must be used for interactive rotation.
 * @author Jean-Baptiste Silvy
 */
public class AxesRotationTracker extends MouseDisplacementTracker implements MouseListener {
	
	private static final String ICON_PATH = System.getenv("SCI") + "/modules/gui/images/icons/rotate.png";
	private static final String CURSOR_ICON_NAME = "zoom-area";
	
	private boolean recordStarted;
	private boolean recordEnded;
	private boolean isWaitingForClick;
	
	private int clickPosX;
	private int clickPosY;
	
	/**
	 * default constructor
	 * @param canvas canvas on which we want to record mouse displacement
	 */
	public AxesRotationTracker(Component canvas) {
		super(canvas);
		reinit();
	}
	
	/**
	 * Reinit mouse tracking
	 */
	protected void reinit() {
		super.reinit();
		this.recordStarted = false;
		this.recordEnded = false;
		this.isWaitingForClick = false;
		clickPosX = -1;
		clickPosY = -1;
		// Back to default cursor
		getTrackedCanvas().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
	}
	
	/**
	 * Wait for a click in the graphic canvas.
	 * @param clickPosition position of the click in pixel
	 */
	public void waitForClick(int[] clickPosition) {
		getTrackedCanvas().addMouseListener(this);
		synchronized (getLock()) {
			isWaitingForClick = true;
			// wait until the click occurs
			try {
				getLock().wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			clickPosition[0] = clickPosX;
			clickPosition[1] = clickPosY;
		}
		getTrackedCanvas().removeMouseListener(this);
	}
	
	/**
	 * Retrieve the displacement performed since last call to the function.
	 * First call to the function waits for click to initialize
	 * @param displacement array [dx, dy] displacement in pixels since last call
	 * @return true if it is still needed to retrieve displacement, false otherwise
	 */
	public boolean getDisplacement(int[] displacement) {
		// Change cursor
		
		if (!recordStarted) {
			// first call
			// change the mouse cursor
			getTrackedCanvas().setCursor(ScilabSwingUtilities.createCursorFromIcon(ICON_PATH, CURSOR_ICON_NAME));
			// wait for initialization with a first click
			waitForClick(displacement);
			if (recordEnded) {
				// the record has been canceled
				reinit();
				return false;
			} else {
				// start recording the mouse displacement
				startRecording(clickPosX, clickPosY);
				return true;
			}
		} else if (!recordEnded) {
			// inside tracking loop
			// get mouse displacement since last call
			getMouseDisplacement(displacement);
			
			if (recordEnded) {
				// record might have been canceled asynchronously
				reinit();
				return false;
			} else {
				return true;
			}
		} else {
			// last call get current displacement and return
			getImmediateMouseDisplacement(displacement);
			
			// reinit for a next call
			reinit();

			return false;
		}
	}
	
	/**
	 * Manual disabling of recording
	 */
	public void cancelRecording() {
		endRecording();
		
		// wake everyone if needed
		synchronized (getLock()) {
			getLock().notifyAll();
		}
	}
	
	/**
	 * Start mouse tracking.
	 * @param initX initial X coordinate
	 * @param initY initial Y coordinate
	 */
	protected void startRecording(int initX, int initY) {
		activateTracking(initX, initY);
		// for final click
		getTrackedCanvas().addMouseListener(this);
		recordStarted = true;
	}
	
	/**
	 * End mouse tracking.
	 */
	protected void endRecording() {
		if (recordStarted) {
			desactivateTracking();
		}
		getTrackedCanvas().removeMouseListener(this);
		recordEnded = true;
		recordStarted = true;
	}
	
	/**
	 * @param event click event
	 */
	public void mouseClicked(MouseEvent event) {
		// everything is done in mouse pressed
		// since it is called first
	}

	/**
	 * @param event entering event
	 */
	public void mouseEntered(MouseEvent event) {
		// not used

	}

	/**
	 * @param event exiting event
	 */
	public void mouseExited(MouseEvent event) {
	    cancelRecording();
	}

	/**
	 * @param event press event
	 */
	public void mousePressed(MouseEvent event) {
		
		if (isWaitingForClick) {
			// the first click is occuring
			// first check if it is a cancel click or a not
			
			if (event.getButton() == MouseEvent.BUTTON1) {
				clickPosX = event.getX();
				clickPosY = event.getY();
				isWaitingForClick = false;
			} else {
				clickPosX = -1;
				clickPosY = -1;
				recordEnded = true;
			}
			
			
		
			// wake the click waiter
			synchronized (getLock()) {
				getLock().notifyAll();
			}
			
		} else if (recordStarted && !recordEnded) {
			// tracking loop
			
			// update position
			// so next call to getDisplacement will get it
			updateDisplacement(event.getX(), event.getY());
			
			// we finish record loop
			endRecording();
		}
	}

	/**
	 * @param event release event
	 */
	public void mouseReleased(MouseEvent event) {
		// nothing to do

	}

}
