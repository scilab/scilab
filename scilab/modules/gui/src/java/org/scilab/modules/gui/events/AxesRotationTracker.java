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
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import org.scilab.modules.gui.utils.Debug;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;


/**
 * Class used to retrieve displacement that must be used for interactive rotation.
 * @author Jean-Baptiste Silvy
 */
public class AxesRotationTracker implements MouseListener, MouseMotionListener, KeyListener {

    private static final String ICON_PATH = System.getenv("SCI") + "/modules/gui/images/icons/rotate.png";
    private static final String CURSOR_ICON_NAME = "zoom-area";

    private boolean recordStarted;
    private boolean recordEnded;
    private boolean isWaitingForClick;

    private int clickPosX;
    private int clickPosY;

    /** Canvas on which the tracking is done */
    private Component canvas;
    /** For synchronization */
    private Object lock;

    /** To know if the displacement has been updated and if we can send them */
    private boolean coordinatesUpdated;

    // coordinates of last call and the one fo current call
    private int previousX;
    private int previousY;
    private int currentX;
    private int currentY;

    /**
     * default constructor
     * @param canvas canvas on which we want to record mouse displacement
     */
    public AxesRotationTracker(Component canvas) {
	this.canvas = canvas;
	this.lock = new Object();
	reinit();
    }

    /**
     * Reinit mouse tracking
     */
    private void reinit() {
	this.recordStarted = false;
	this.recordEnded = false;
	this.isWaitingForClick = false;
	this.coordinatesUpdated = false;
	this.previousX = -1;
	this.previousY = -1;
	this.currentX = -1;
	this.currentY = -1;
	clickPosX = -1;
	clickPosY = -1;
	// Back to default cursor
	canvas.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    }

    /**
     * Wait for a click in the graphic canvas.
     * @param clickPosition position of the click in pixel
     */
    public void waitForClick(int[] clickPosition) {
	canvas.addMouseListener(this);
	canvas.addKeyListener(this);
	
	synchronized (lock) {
	    isWaitingForClick = true;
	    // wait until the click occurs
	    try {
		lock.wait();
	    } catch (InterruptedException e) {
		e.printStackTrace();
	    }
	    clickPosition[0] = clickPosX;
	    clickPosition[1] = clickPosY;
	}
    }

    /**
     * Retrieve displacement since the last call of getDisplacement.
     * If no displacement has been notified then wait until one occures.
     * @param displacement array [dx, dy] displacement in pixels
     */
    private void getMouseDisplacement(int[] displacement) {
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
     * Retrieve the displacement performed since last call to the function.
     * First call to the function waits for click to initialize
     * @param displacement array [dx, dy] displacement in pixels since last call
     * @return true if it is still needed to retrieve displacement, false otherwise
     */
    public boolean getDisplacement(int[] displacement) {
	if (!recordStarted && !recordEnded) {
	 // change the mouse cursor
	    canvas.setCursor(ScilabSwingUtilities.createCursorFromIcon(ICON_PATH, CURSOR_ICON_NAME));
	    // wait for initialization with a first click
	    waitForClick(displacement);
	    // start recording the mouse displacement
	    startRecording(clickPosX, clickPosY);
	    Debug.DEBUG("return true");
	    return true;
	}
	
	if (!recordEnded) {
	    // inside tracking loop
	    // get mouse displacement since last call
	    getMouseDisplacement(displacement);
	    return true;
	}
	 
	 getImmediateMouseDisplacement(displacement);
	 // reinit for a next call
	 reinit();
	 return false;
    }

    /**
     * Just get the current displacement but does not perform any update
     * @param displacement array [dx, dy] displacement in pixels
     */
    private void getImmediateMouseDisplacement(int[] displacement) {
	synchronized (lock) {
	    // get displacement
	    displacement[0] = currentX - previousX;
	    displacement[1] = currentY - previousY;
	}
    }

    /**
     * Manual disabling of recording
     */
    public void cancelRecording() {
	// reset Position to the starting one.
	previousX = currentX;
	previousY = currentY;
	currentX = clickPosX;
	currentY = clickPosY;
	endRecording();
    }

    /**
     * Start mouse tracking.
     * @param initX initial X coordinate
     * @param initY initial Y coordinate
     */
    private void startRecording(int initX, int initY) {
	previousX = initX;
	previousY = initY;
	// for final click
	canvas.addMouseMotionListener(this);
	recordStarted = true;
    }

    /**
     * End mouse tracking.
     */
    private void endRecording() {
	canvas.removeMouseMotionListener(this);
	canvas.removeMouseListener(this);
	canvas.removeKeyListener(this);
	recordEnded = true;
	recordStarted = true;
	// wake everyone if needed
	synchronized (lock) {
	    lock.notifyAll();
	}
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
	    synchronized (lock) {
		lock.notifyAll();
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
    private void updateDisplacement(int xPos, int yPos) {
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

    /**
     * Key Listener
     * {
     */
    
    /**
     * keyPressed
     * @param arg0 : the key event.
     * If ESC is pressed, just cancel rotation.
     */
    public void keyPressed(KeyEvent arg0) {
	if (arg0.getKeyCode() == KeyEvent.VK_ESCAPE) {
	    cancelRecording();
	}
    }
    
    /**
     * keyReleased
     * @param arg0 : the key event.
     * DO NOTHING.
     */
    public void keyReleased(KeyEvent arg0) { }
    
    /**
     * keyTyped
     * @param arg0 : the key event.
     * DO NOTHING.
     */
    public void keyTyped(KeyEvent arg0) { }
   
    /**
     * }
     */

}
