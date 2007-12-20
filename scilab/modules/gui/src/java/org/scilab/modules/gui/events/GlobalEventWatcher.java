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
	private GlobalEventWatcher() {	}
	
	/**
	 * Singleton enable.
	 * 
	 * @return the global watcher instance
	 */	
	public static GlobalEventWatcher getInstance() {
		if (me == null) {
			me = new GlobalEventWatcher();
		}
		return me;
	}
	
	
	/**
	 * Singleton enable keyWatcher.
	 * 
	 * @param keyWatcher : the Key Listener.
	 */
	public static void enable(GlobalKeyEventWatcher keyWatcher) {
		GlobalEventWatcher.keyWatcher = keyWatcher;
		Toolkit.getDefaultToolkit().addAWTEventListener(keyWatcher,
				AWTEvent.KEY_EVENT_MASK);
		GlobalEventWatcher.activated = true;
	}

	/**
	 * Singleton enable keyWatcher.
	 * 
	 * @param mouseWatcher : the Mouse Listener.
	 */
	public static void enable(GlobalMouseEventWatcher mouseWatcher) {
		GlobalEventWatcher.mouseWatcher = mouseWatcher;
		Toolkit.getDefaultToolkit().addAWTEventListener(mouseWatcher,
				mouseWatcher.getEventMask());
			GlobalEventWatcher.activated = true;
	}

	
	/**
	 * Disable the global watcher
	 */
	public static void disable() {
		Toolkit.getDefaultToolkit().removeAWTEventListener(GlobalEventWatcher.keyWatcher);
		Toolkit.getDefaultToolkit().removeAWTEventListener(GlobalEventWatcher.mouseWatcher);
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
