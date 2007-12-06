package org.scilab.modules.gui.events.callback;


import java.awt.event.ActionEvent;

/**
 * Generic class to hide callback management
 * @author bruno
 *
 */
public abstract class JavaCallBack extends CallBack {
	
	/**
	 * Standard Java behaviour encapsulated behind callback
	 * which is more Scilab compliant.
	 * @param e The action event that awake the callback
	 */
	public void actionPerformed(ActionEvent e) {
		callBack();
	}
}
