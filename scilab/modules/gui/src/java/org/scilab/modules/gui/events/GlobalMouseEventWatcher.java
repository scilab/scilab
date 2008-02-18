package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno Jofret

 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 * This class create a global mouse event watcher
 * means at a very high level in the JVM.
 * @author Bruno Jofret
 */
public abstract class GlobalMouseEventWatcher implements AWTEventListener {

	/**
	 * Those static final constant are only for reverse compatibility
	 * With old Scilab.
	 * Java button are :
	 * 1 = left
	 * 2 = middle
	 * 3 = right
	 * We add those statics state integers such as
	 * button + state = scilab return value.
	 * 
	 * ex : left PRESSED = 1 + (-1) = 0
	 * 
	 * MOVE does not feat the rule because 
	 * it does not have any Button associated with.
	 */
	/** Internal state PRESSED */ 
	public static final int PRESSED = -1;
	/** Internal state RELEASED */
	public static final int RELEASED = -6;
	/** Internal state CLICKED */
	public static final int CLICKED = 2;
	/** Internal state DCLICKED */
	public static final int DCLICKED = 9;
	/** Internal state MOVED */
	public static final int MOVED = -100;

	private static final int UNMANAGED = -10000;
	private static final int TIMETOSLEEP = 300;

	private boolean isControlDown;
	private boolean inCanvas;
	private boolean freedom = true;
	private int action = UNMANAGED;
	private long eventMask;
	private MouseEvent lastMouse;
	private SwingScilabCanvas canvas;

	/**
	 * Constructor.
	 * 
	 * @param eventMask : a Mask for mouse event watching.
	 * xclick is AWTEvent.MOUSE_EVENT_MASK
	 * xgetmouse is AWTEvent.MOUSE_EVENT_MASK + AWTEvent.MOUSE_MOTION_EVENT_MASK
	 */
	public GlobalMouseEventWatcher(long eventMask) {
		this.eventMask = eventMask;
	}

	/** 
	 * 
	 * When there is a mouse click on a Canvas, just go through the GlobalFilter.
	 * @param mouseEvent : The mouse event caught.
	 * 
	 * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
	 */
	public void eventDispatched(AWTEvent mouseEvent) {
		// DEBUG
		//System.out.println(((MouseEvent) mouseEvent).toString());

		/*
		 * Managing Canvas
		 * PRESSED
		 * CLICKED
		 * DCLICKED
		 * MOVED
		 */
		this.lastMouse = (MouseEvent) mouseEvent;
		if (mouseEvent.getSource() instanceof SwingScilabCanvas) {
			this.canvas = (SwingScilabCanvas) mouseEvent.getSource();
			this.isControlDown = lastMouse.isControlDown();
			switch (mouseEvent.getID()) {
			/* CLICKED */
			case MouseEvent.MOUSE_CLICKED :
				if (lastMouse.getClickCount() == 1) {
					setAction(CLICKED);
				} else {
					/*
					 * Means mouseEvent.getClickCount() >= 2
					 */ 
					setAction(DCLICKED);
					synchronized (this) {
						this.notify();
					}
				}		
				break;
				/* PRESSED */
			case MouseEvent.MOUSE_PRESSED :
				setAction(PRESSED);
				if (this.freedom) {
					Thread timer = new Thread() {
						public void run() { launchFilter(); } 
					};
					timer.start();
				}			
				break;
				/* ENTERED */
			case MouseEvent.MOUSE_ENTERED :
				this.inCanvas = true;
				mouseEventFilter(lastMouse, canvas, MOVED, this.isControlDown);
				break;
				/* MOVED */
			case MouseEvent.MOUSE_MOVED :
				mouseEventFilter(lastMouse, canvas, MOVED, this.isControlDown);
				break;
			case MouseEvent.MOUSE_DRAGGED :
				mouseEventFilter(lastMouse, canvas, MOVED, this.isControlDown);
				break;
				/* EXITED */
			case MouseEvent.MOUSE_EXITED :
				this.inCanvas = false;
				break;
			default:
				break;
			}
		} else {
		/*
		 * Manage RELEASED on a Canvas
		 * Means we are still in a Canvas (ENTERED && !EXITED)
		 * and the event is not comming from a Canvas itself.
		 * and got a RELEASED
		 */
			if (mouseEvent.getID() == MouseEvent.MOUSE_RELEASED && inCanvas) {
				this.action = RELEASED;
				mouseEventFilter(lastMouse, canvas, this.action, this.isControlDown);	
			}
		}
	}

	/**
	 * Setter for action
	 * (just in case we need a lock in the future)
	 * @param newAction : the Action to remember.
	 */
	private void setAction(int newAction) {
		this.action = newAction;
	}


	/**
	 * Thread to manage old style scilab
	 * click management.
	 */
	public void launchFilter() {
		this.freedom = false;
		try {
			synchronized (this) {
				wait(TIMETOSLEEP);
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		mouseEventFilter(lastMouse, canvas, this.action, this.isControlDown);
		this.freedom = true;
		this.action = UNMANAGED;		
	}

	/**
	 * Method to filter the event received.
	 * Depends off what kind of function is called.
	 * 
	 * @param mouseEvent : the mouse event caught (as seen in Scilab) 
	 * @param canvas : the canvas where action occurs.
	 * @param scilabMouseAction : the integer scilab code for mouse action.
	 * @param isControlDown true if the CTRL key has been pressed
	 */
	public abstract void mouseEventFilter(MouseEvent mouseEvent, SwingScilabCanvas canvas, int scilabMouseAction, boolean isControlDown);

	/**
	 * Event Mask getter.
	 * 
	 * @return eventMask
	 */
	public long getEventMask() {
		return eventMask;
	}

}
