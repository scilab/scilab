package org.scilab.modules.gui.events.callback;

import java.awt.event.ActionEvent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.events.GlobalEventWatcher;

public abstract class ScilabCallBack extends CallBack {

	public ScilabCallBack() {}
	
	public ScilabCallBack(String command, int type) {
		this.command = command;
		this.type = type;
	}
	
	public static ScilabCallBack create(String command, int type) {
		return (new ScilabCallBack(command, type) {
				public void callBack() {
					this.storeCommand(this.getCommand(), this.getType());
				}
		});
	}
	
	public void actionPerformed(ActionEvent e) {
		if (!GlobalEventWatcher.isActivated()) { 
			// DEBUG
			// System.out.println("Action Performed : CallBack on ["+ e.getSource().getClass().getName()+"]");
				callBack();
		}
	}
	
	public void storeCommand(String command, int type) {
		InterpreterManagement.putCommandInScilabQueue(command);
	}

	public String getCommand() {
		return command;
	}
	
	public int getType() {
		return type;
	}
	
	private String command;
	private int type;
}
