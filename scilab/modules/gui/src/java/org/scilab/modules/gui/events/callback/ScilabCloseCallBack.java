/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
	 * @param figureIndex : the figure ID where callback occured.
	 * @param command : the command to execute.
	 */
	private ScilabCloseCallBack(int figureIndex, String command) {
		super(command);
		this.figureIndex = figureIndex;
	}

	/**
	 * Callback Factory to easily create a callback
	 * just like in scilab.
	 * @param figureIndex : the figure ID where callback occured.
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
