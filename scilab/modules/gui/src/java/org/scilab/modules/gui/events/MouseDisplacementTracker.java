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

import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * Class used to retrieve synchronous mouse displacements
 * @author Jean-Baptiste Silvy
 */
public class MouseDisplacementTracker implements MouseMotionListener {

	/** Canvas on which the tracking is done */
	private SwingScilabCanvas canvas;
	/** For syncrnonisation */
	private Object lock;
	
	/** To know if the displacement has been updated and if we can send them */
	private boolean coordinatesUpdated;
	
	// coordinates of last call and the one fo current call
	private int previousX;
	private int previousY;
	private int currentX;
	private int currentY;
	
	
	/**
	 * Defautl constructor
	 * @param canvas Canvas on which the tracking is done
	 */
	public MouseDisplacementTracker(SwingScilabCanvas canvas) {
		this.canvas = canvas;
		this.lock = new Object();
		reinit();
	}
	
	/**
	 * @return tracked canvas
	 */
	protected SwingScilabCanvas getTrackedCanvas() {
		return canvas;
	}
	
	/**
	 * @return synchrnonization object
	 */
	protected Object getLock() {
		return lock;
	}
	
	/**
	 * Reinit mouse tracking
	 */
	protected void reinit() {
		this.coordinatesUpdated = false;
		this.previousX = -1;
		this.previousY = -1;
		this.currentX = -1;
		this.currentY = -1;
	}
	
	/**
	 * Activate recording of mouse movement
	 * @param initX intial X coordinate
	 * @param initY intial Y coordinate
	 */
	protected void activateTracking(int initX, int initY) {
		previousX = initX;
		previousY = initY;
		canvas.addMouseMotionListener(this);
	}
	
	/**
	 * Desactivate recording of mouse movement
	 */
	protected void desactivateTracking() {
		canvas.removeMouseMotionListener(this);
	}
	
	/**
	 * Retrieve displacement since the last call of getDisplacement.
	 * If no displacement has been notified then wait until one occures.
	 * @param displacement array [dx, dy] displacement in pixels
	 */
	public void getMouseDisplacement(int[] displacement) {
		
		synchronized (lock) {
			if (!coordinatesUpdated) {
				// no new displacement, wait for one
				try {
					lock.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			
			// get displacement
			displacement[0] = currentX - previousX;
			displacement[1] = currentY - previousY;
			
			// update previous coordinates
			previousX = currentX;
			previousY = currentY;
			
			coordinatesUpdated = false;
		}
	}
	
	/**
	 * Just get the current displacement but does not perform any update
	 * @param displacement array [dx, dy] displacement in pixels
	 */
	protected void getImmediateMouseDisplacement(int[] displacement) {
		synchronized (lock) {
			// get displacement
			displacement[0] = currentX - previousX;
			displacement[1] = currentY - previousY;
		}
	}
	
	/**
	 * @param event dragging event
	 */
	public void mouseDragged(MouseEvent event) {
		// same as mouse moved
		mouseMoved(event);
	}
	

	/**
	 * @param event move event
	 */
	public void mouseMoved(MouseEvent event) {
		updateDisplacement(event.getX(), event.getY());
	}
	
	/**
	 * Update displacement with a new mouse position
	 * @param xPos X coordinate of the mouse
	 * @param yPos Y coordinate of the mouse
	 */
	protected void updateDisplacement(int xPos, int yPos) {
		synchronized (lock) {
			// record new position
			currentX = xPos;
			currentY = yPos;
			
			coordinatesUpdated = true;
			
			// wake threads waiting for a displacement
			// if some
			lock.notifyAll();
		}
	}

}
