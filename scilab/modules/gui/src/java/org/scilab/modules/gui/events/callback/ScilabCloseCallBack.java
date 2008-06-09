package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;
import org.scilab.modules.gui.utils.SciTranslator;

/**
 * ScilabCallback abstract class to easily manage callbacks
 * that throws commands to Scilab.
 *
 * @author Bruno JOFRET
 *
 */
public abstract class ScilabCloseCallBack extends CallBack {

	private int figureIndex;
	
	/**
	 * Constructor
	 * @param command : the command to execute.
	 */
	private ScilabCloseCallBack(int figureIndex, String command) {
		super(command);
		this.figureIndex = figureIndex;
	}

	/**
	 * Callback Factory to easily create a callback
	 * just like in scilab.
	 * @param command : the command to execute.
	 * @return a usable Scilab callback
	 */
	public static ScilabCloseCallBack create(int figureIndex, String command) {
		return (new ScilabCloseCallBack(figureIndex, command) {

			private static final long serialVersionUID = -7286803341046313407L;

			public void callBack() {
				Thread launchMe = new Thread() {
					public void run() {
						InterpreterManagement.putCommandInScilabQueue(getCommand());
					}
				};
				launchMe.start();
			}
		});
	}

	/**
	 * To match the standard Java Action management.
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 * @param e The event that launch the callback.
	 */
	public void actionPerformed(ActionEvent e) {
	/**
	 * !! WARNING !!
	 * 
	 * We need a special behaviour when asking the window to close.
	 * We really need to close it (i.e exec the callback)
	 * THEN to inform the global listener close command has been activated (i.e filterCallback).
	 * 
	 */
		callBack();
		if (GlobalEventWatcher.isActivated()) {
			if (this.command != null) {
				GlobalEventFilter.filterCallback(this.command, SciTranslator.SCICLOSE, figureIndex);
			}
		}
	}
}
