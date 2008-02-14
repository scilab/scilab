package org.scilab.modules.gui.events.callback;


import java.awt.event.ActionEvent;
import java.lang.reflect.Method;

import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;

/**
 * Generic class to hide callback management
 * @author bruno
 *
 */
public abstract class JavaCallBack extends CallBack {

	/**
	 * @param command : the command to execute.
	 */
	private JavaCallBack(String command) {
		super(command);
	}

	/**
	 * Callback Factory to easily create a callback
	 * just like in scilab.
	 * @param command : the command to execute.
	 * @return a usable Java callback
	 */
	public static JavaCallBack create(String command) {
		return (new JavaCallBack(command) {
			public void callBack() {
				try {
					int lastPoint = command.lastIndexOf(".");
					Class invokedClass = Class.forName(command.substring(0, lastPoint));
					Method runMe = invokedClass.getMethod(command.substring(lastPoint + 1));
					// Only able to launch method Class.
					runMe.invoke(invokedClass.getClass(), (Object[]) null);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
	}
	/**
	 * Standard Java behaviour encapsulated behind callback
	 * which is more Scilab compliant.
	 * @param e The action event that awake the callback
	 */
	public void actionPerformed(ActionEvent e) {
		if (!GlobalEventWatcher.isActivated()) {
			callBack();
		} else {
			if (this.command != null) {
				GlobalEventFilter.filterCallback(e, this.command);
			}
		}
	}
}
