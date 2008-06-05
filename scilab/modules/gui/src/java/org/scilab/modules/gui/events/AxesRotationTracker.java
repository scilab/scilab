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

import java.awt.Cursor;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * Class used to retrieve displacement that must be used for interactive rotation.
 * @author Jean-Baptiste Silvy
 */
public class AxesRotationTracker extends MouseDisplacementTracker implements MouseListener, FocusListener {
	
	private boolean recordStarted;
	private boolean recordEnded;
	private boolean isWaitingForClick;
	
	private int clickPosX;
	private int clickPosY;
	
	/**
	 * default constructor
	 * @param canvas canvas on which we want to recod mouse displacement
	 */
	public AxesRotationTracker(SwingScilabCanvas canvas) {
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
		getTrackedCanvas().addFocusListener(this);
		synchronized (getLock()) {
			isWaitingForClick = true;
			// wait until the click occures
			try {
				getLock().wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			clickPosition[0] = clickPosX;
			clickPosition[1] = clickPosY;
		}
		getTrackedCanvas().removeMouseListener(this);
		getTrackedCanvas().removeFocusListener(this);
	}
	
	/**
	 * Retrieve the displacement performed since last call to the function.
	 * First call to the function waits for clivk to initialize
	 * @param displacement array [dx, dy] displacement in pixels since last call
	 * @return true if it is still needed to retrieve displacement, false otherwise
	 */
	public boolean getDisplacement(int[] displacement) {
		// Change cursor
		
		if (!recordStarted) {
			// first call
			Image icon = Toolkit.getDefaultToolkit().getImage(System.getenv("SCI") + "/modules/gui/images/icons/rotate.png");
			getTrackedCanvas().setCursor(Toolkit.getDefaultToolkit().createCustomCursor(icon, new Point(0, 0), "rotate"));
			waitForClick(displacement);
			startRecording(clickPosX, clickPosY);
			return true;
		} else if (!recordEnded) {
			// inside tracking loop
			// get mouse displacement since last call
			getMouseDisplacement(displacement);
			return true;
		} else {
			// last call get current displacement and return
			getImmediateMouseDisplacement(displacement);
			
			// reinit for a next call
			reinit();

			return false;
		}
	}
	
	/**
	 * Manual disactivation of recording
	 */
	public void cancelRecording() {
		endRecording();
		reinit();
	}
	
	/**
	 * Start mouse tarcking.
	 * @param initX initial X coordinate
	 * @param initY initial Y coordinate
	 */
	protected void startRecording(int initX, int initY) {
		activateTracking(initX, initY);
		// for final click
		getTrackedCanvas().addMouseListener(this);
		getTrackedCanvas().addFocusListener(this);
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
		getTrackedCanvas().removeFocusListener(this);
		recordEnded = true;
	}
	
	/**
	 * @param event clieck event
	 */
	public void mouseClicked(MouseEvent event) {
		// everything si done in mouse pressed
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
		// not used

	}

	/**
	 * @param event press event
	 */
	public void mousePressed(MouseEvent event) {
		if (isWaitingForClick) {
			clickPosX = event.getX();
			clickPosY = event.getY();
			isWaitingForClick = false;
			
			// wake the click waiter
			synchronized (getLock()) {
				getLock().notifyAll();
			}
			
			return;
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
	
	/**
	 * @param event focus gained event
	 */
	public void focusGained(FocusEvent event) {
		// nothing to do here
		// canvas must always have focus
	}

	/**
	 * This event occures when the canvas lost focus but
	 * also when the windows is closed. We then need to wake up every one.
	 * @param event focus lost event
	 */
	public void focusLost(FocusEvent event) {
		// focus lost so stop recording
		endRecording();
		synchronized (getLock()) {
			getLock().notifyAll();
		}
	}

}
