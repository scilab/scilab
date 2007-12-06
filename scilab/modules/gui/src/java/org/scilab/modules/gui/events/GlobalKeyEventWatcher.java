package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.KeyEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

public class GlobalKeyEventWatcher implements AWTEventListener {

	public void eventDispatched(AWTEvent keyEvent) {
		if (keyEvent.getSource() instanceof SwingScilabCanvas) {
			if (GlobalEventWatcher.isActivated()) {
				GlobalEventFilter.filter((KeyEvent) keyEvent);
			}
		} 
	}
}
