package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.Toolkit;

/**
 * This implements a Global event watcher,
 * means a very high level one : quite the same level as the JVM
 * 
 * @author bruno
 *
 */
public final class GlobalEventWatcher {

	private static GlobalEventWatcher me;
	private static GlobalKeyEventWatcher keyWatcher;
	private static GlobalMouseEventWatcher mouseWatcher;
	private static boolean activated;

	/**
	 * Private Constructor : SINGLETON.
	 */
	private GlobalEventWatcher() {
		keyWatcher = new GlobalKeyEventWatcher();
		mouseWatcher = new GlobalMouseEventWatcher();
	}

	/**
	 * Singleton enable.
	 * 
	 * @return the global watcher instance
	 */
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

	/**
	 * Disable the global watcher
	 */
	public static void disable() {
		Toolkit.getDefaultToolkit().removeAWTEventListener(keyWatcher);
		Toolkit.getDefaultToolkit().removeAWTEventListener(mouseWatcher);
		activated = false;
	}

	/**
	 * Is the GlobalWatcher active or not.
	 * @return activation status.
	 */
	public static boolean isActivated() {
		return activated;
	}

}
