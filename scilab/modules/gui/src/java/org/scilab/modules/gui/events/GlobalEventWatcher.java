package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.Toolkit;

public class GlobalEventWatcher {

	private static GlobalEventWatcher me = null;
	private static GlobalKeyEventWatcher keyWatcher = null;
	private static GlobalMouseEventWatcher mouseWatcher = null;
	public static boolean activated = false;

	private GlobalEventWatcher() {
		keyWatcher = new GlobalKeyEventWatcher();
		mouseWatcher = new GlobalMouseEventWatcher();
	}

	public static GlobalEventWatcher enable() {
		if (me == null) {
			me = new GlobalEventWatcher();
		}
		if (!activated) {
			Toolkit.getDefaultToolkit().addAWTEventListener(keyWatcher,
					AWTEvent.KEY_EVENT_MASK);
			Toolkit.getDefaultToolkit().addAWTEventListener(mouseWatcher,
					AWTEvent.MOUSE_EVENT_MASK);
			activated = true;
		}
		return me;
	}

	public static void disable() {
		Toolkit.getDefaultToolkit().removeAWTEventListener(keyWatcher);
		Toolkit.getDefaultToolkit().removeAWTEventListener(mouseWatcher);
		activated = false;
	}

	public static boolean isActivated() {
		return activated;
	}

}
