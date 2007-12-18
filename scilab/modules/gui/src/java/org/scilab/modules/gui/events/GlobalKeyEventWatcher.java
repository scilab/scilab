package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.Toolkit;
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

	private int keyChar;
	private boolean isCtrlDown;
	
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
		KeyEvent keyEvent = (KeyEvent) event;
		System.out.println(keyEvent.toString());
		if(keyEvent.getID() == KeyEvent.KEY_PRESSED) {
			if (Character.isJavaIdentifierStart(keyEvent.getKeyChar())) {
				this.keyChar = keyEvent.getKeyChar();
			}
			else {
				// Serge, j'aurai ta peau !!!!!!!!!
				if (keyEvent.isShiftDown()) {
					this.keyChar = keyEvent.getKeyCode();
				}
				else {
					this.keyChar = Character.toLowerCase(keyEvent.getKeyCode());
				}
			}
			this.isCtrlDown = ((KeyEvent) keyEvent).isControlDown();
		}
		else if(keyEvent.getID() == KeyEvent.KEY_TYPED) {	
			if (keyEvent.getSource() instanceof SwingScilabCanvas) {
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filterKey(keyChar, isCtrlDown);
				}
			}
		} 
	}
}
