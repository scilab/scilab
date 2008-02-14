package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;

/**
 * ScilabCallback abstract class to easily manage callbacks
 * that throws commands to Scilab.
 *
 * @author Bruno JOFRET
 *
 */
public abstract class ScilabCallBack extends CallBack {

	/**
	 * Constructor
	 * @param command : the command to execute.
	 */
	private ScilabCallBack(String command) {
		super(command);
	}

	/**
	 * Callback Factory to easily create a callback
	 * just like in scilab.
	 * @param command : the command to execute.
	 * @return a usable Scilab callback
	 */
	public static ScilabCallBack create(String command) {
		return (new ScilabCallBack(command) {

			private static final long serialVersionUID = -7286803341046313407L;

			public void callBack() {
				this.storeCommand(this.getCommand());
			}
		});
	}

	/**
	 * To match the standard Java Action management.
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 * @param e The event that launch the callback.
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

	/**
	 * Put the command recieved through the callback.
	 *
	 * @param command : The command to throw to Scilab
	 */
	public void storeCommand(String command) {
		this.command = command;
		Thread launchMe = new Thread() {
			public void run() {
				ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(getCommand(), false, false);
			}
		};
		launchMe.start();
	}
}
