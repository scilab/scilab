package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.events.GlobalEventFilter;
import org.scilab.modules.gui.events.GlobalEventWatcher;

/**
 * ScilabCallback abstract class to easily manage callbacks
 * that throws commands to Scilab.
 *
 * @author bruno
 *
 */
public abstract class ScilabCallBack extends CallBack {

	// @TODO : Delete me I'm useless...
	private int type;

	/**
	 * @param command : the command to execute.
	 * @param type : the type of this command.
	 */
	private ScilabCallBack(String command, int type) {
		super(command);
		this.type = type;
	}

	/**
	 * Callback Factory to easily create a callback
	 * just like in scilab.
	 * @param command : the command to execute.
	 * @param type : the type of this command.
	 * @return a usable Java callback
	 */
	public static ScilabCallBack create(String command, int type) {
		return (new ScilabCallBack(command, type) {

			private static final long serialVersionUID = -7286803341046313407L;

			public void callBack() {
				this.storeCommand(this.getCommand(), this.getType());
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
	 * @param type : The type of the command, C or Fortran compiled code, Scilab instruction...
	 */
	public void storeCommand(String command, int type) {
		this.command = command;
		Thread launchMe = new Thread() {
			public void run() {
				ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(getCommand(), false, false);
			}
		};
		launchMe.start();
	}

	/**
	 * @return The type of the Scilab callback
	 */
	public int getType() {
		return type;
	}
}
