package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.Toolkit;
import java.awt.event.AWTEventListener;

public class GlobalEventWatcher implements AWTEventListener {

	private static GlobalEventWatcher me = null;
	private static boolean activated = false;
	
	private GlobalEventWatcher() {}
		
	public static GlobalEventWatcher enable() {
		if (me == null) {
			me = new GlobalEventWatcher();
		}
		if (!activated) {
			Toolkit.getDefaultToolkit().addAWTEventListener(
					me,
					AWTEvent.ACTION_EVENT_MASK + AWTEvent.FOCUS_EVENT_MASK
							+ AWTEvent.MOUSE_MOTION_EVENT_MASK
							+ AWTEvent.MOUSE_EVENT_MASK
							+ AWTEvent.MOUSE_WHEEL_EVENT_MASK);
			activated = true;
		}
		return me;
	}
	
	public static void disable() {
		Toolkit.getDefaultToolkit().removeAWTEventListener(me);
		activated = false;
	}
	
	public static boolean isActivated() {
		return activated;
	}
	
	@Override
	public void eventDispatched(AWTEvent event) {
		GlobalEventFilter.filter(event.getSource().getClass(), event);
	}

}
