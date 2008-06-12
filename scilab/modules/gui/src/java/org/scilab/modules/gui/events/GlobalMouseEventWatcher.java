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

package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.ScrollabeSwingScilabCanvas;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.SciTranslator;

/**
 * This class create a global mouse event watcher
 * means at a very high level in the JVM.
 * @author Bruno Jofret
 */
public abstract class GlobalMouseEventWatcher implements AWTEventListener {




	private boolean isControlDown;
	private boolean inCanvas = true;
	private boolean freedom = true;
	private long eventMask;
	private SciTranslator clickTranslator;
	private MouseEvent lastMouse;
	private static SwingScilabCanvas canvas = null;

	/**
	 * Constructor.
	 * 
	 * @param eventMask : a Mask for mouse event watching.
	 * xclick is AWTEvent.MOUSE_EVENT_MASK
	 * xgetmouse is AWTEvent.MOUSE_EVENT_MASK + AWTEvent.MOUSE_MOTION_EVENT_MASK
	 */
	public GlobalMouseEventWatcher(long eventMask) {
		this.eventMask = eventMask;
		//this.canvas = null;
		clickTranslator = new SciTranslator();
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
			/** ADD COMMENT */
			if (canvas == null) {
				this.canvas = (SwingScilabCanvas) mouseEvent.getSource();
			}
			this.isControlDown = lastMouse.isControlDown();
			switch (mouseEvent.getID()) {
			/* CLICKED */
			case MouseEvent.MOUSE_CLICKED :
				this.inCanvas = true;
				if (lastMouse.getClickCount() == 1) {
					clickTranslator.setClickAction(SciTranslator.CLICKED);
				} 
				else {
					/*
					 * Means mouseEvent.getClickCount() >= 2
					 */ 
					clickTranslator.setClickAction(SciTranslator.DCLICKED);
					synchronized (clickTranslator) {
						// To unlock javaClick2Scilab done in launchfilter
						clickTranslator.notify();
					}
				}		
				break;
				/* PRESSED */
			case MouseEvent.MOUSE_PRESSED :
				this.inCanvas = true;
				clickTranslator.setClickAction(SciTranslator.PRESSED);
				if (this.freedom) {
					Thread timer = new Thread() {
						public void run() { 
							launchFilter(); 
						} 
					};
					timer.start();
				}
				break;
				/* ENTERED */
			case MouseEvent.MOUSE_ENTERED :
				this.inCanvas = true;
				this.canvas = (SwingScilabCanvas) mouseEvent.getSource();
				mouseEventFilter(lastMouse, canvas, SciTranslator.MOVED, this.isControlDown);
				break;
				/* MOVED */
			case MouseEvent.MOUSE_MOVED :
				this.inCanvas = true;
				mouseEventFilter(lastMouse, canvas, SciTranslator.MOVED, this.isControlDown);
				break;
			case MouseEvent.MOUSE_DRAGGED :
				if (this.inCanvas) {
					mouseEventFilter(lastMouse, canvas, SciTranslator.MOVED, this.isControlDown);
				}
				break;
				/* EXITED */
			case MouseEvent.MOUSE_EXITED :
				this.inCanvas = false;
				this.canvas = null;
				break;
			default:
				break;
			}

		}
		if (mouseEvent.getSource() instanceof ScrollabeSwingScilabCanvas) {
			switch (mouseEvent.getID()) {
			case MouseEvent.MOUSE_ENTERED :
				this.inCanvas = true;
				//this.canvas = (SwingScilabCanvas) mouseEvent.getSource();
				//mouseEventFilter(lastMouse, canvas, SciTranslator.MOVED, this.isControlDown);
				break;
			case MouseEvent.MOUSE_EXITED :
				this.inCanvas = false;
				this.canvas = null;
				break;
			default:
				break;
			}
		}

		/*
		 * Manage RELEASED on a Canvas
		 * Means we are still in a Canvas (ENTERED && !EXITED)
		 * and the event is not comming from a Canvas itself.
		 * and got a RELEASED
		 */
		if (mouseEvent.getID() == MouseEvent.MOUSE_RELEASED && inCanvas 
				&& (clickTranslator.getClickAction() == SciTranslator.UNMANAGED || clickTranslator.getClickAction() == SciTranslator.MOVED)) {
			clickTranslator.setClickAction(SciTranslator.RELEASED);
			mouseEventFilter(lastMouse, canvas, clickTranslator.getClickAction(), this.isControlDown);	
		}
	}

	/**
	 * Thread to manage old style scilab
	 * click management.
	 */
	private void launchFilter() {
		this.freedom = false;
		mouseEventFilter(lastMouse, canvas, clickTranslator.javaClick2Scilab(), this.isControlDown);
		this.freedom = true;
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
