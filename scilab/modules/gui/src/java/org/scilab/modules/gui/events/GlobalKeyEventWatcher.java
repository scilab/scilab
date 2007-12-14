package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.KeyEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * Global key event watcher.
 * 
 * @author bruno
 *
 */
public class GlobalKeyEventWatcher implements AWTEventListener {

	/**
	 *  Constructor.
	 */
	public GlobalKeyEventWatcher() { }
	
	/** 
	 * Global key event watcher.
	 * 
	 * @param keyEvent : The Key event caught.
	 * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
	 */
	public void eventDispatched(AWTEvent keyEvent) {
		if(keyEvent.getID() == KeyEvent.KEY_TYPED) {	
			if (keyEvent.getSource() instanceof SwingScilabCanvas) {
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filter((KeyEvent) keyEvent);
				}
			}
		} 
	}
}
