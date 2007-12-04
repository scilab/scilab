package org.scilab.modules.gui.events;

import java.awt.AWTEvent;

import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.menu.ScilabMenu;

public class GlobalEventFilter {

	public static void filter(Object class1, AWTEvent event) {
		// Do nothing because we do not care of that kind of object.
	}
	
	public static void filter(ScilabCanvas class1, AWTEvent event) {
		System.out.println("** Got an event on a canvas");
		GlobalEventWatcher.disable();
	}

	public static void filter(ScilabMenu class1, AWTEvent event) {
		System.out.println("** Got an event on a menu");
		GlobalEventWatcher.disable();
	}
	
}
