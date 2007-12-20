package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.KeyEvent;


/**
 * Global key event watcher.
 * 
 * @author bruno
 *
 */
public abstract class GlobalKeyEventWatcher implements AWTEventListener {

	/**
	 *  Constructor.
	 */
	public GlobalKeyEventWatcher() { }
	
	/** 
	 * Global key event watcher.
	 * 
	 * @param event : The Key event caught.
	 * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
	 */
	public void eventDispatched(AWTEvent event) {
		keyEventFilter((KeyEvent) event);
	}
	
	/**
	 * Method to filter the event received.
	 * Depends off what kind of function is called.
	 * 
	 * @param keyEvent : the key event caught 
	 */
	public abstract void keyEventFilter(KeyEvent keyEvent);
}
