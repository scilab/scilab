/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Implements useful methods to communicate with the Scilab interpreter.
 */
public class XcosInterpreterManagement extends InterpreterManagement {

	private static Executor executor = Executors.newSingleThreadExecutor();

	private XcosInterpreterManagement() {
		// full static class so private constructor
	}

	/**
	 * Execute the command and wait for the end of the execution.
	 * 
	 * @param command
	 *            The scilab command
	 * @return Status of the execution
	 */
	public static int SynchronousScilabExec(String command) {
		final String uid = Integer.toString(command.hashCode());
		
		command += ";xcosNotify(\"" + uid + "\");";
		int ret = InterpreterManagement.requestScilabExec(command);
		Signal.wait(uid);
		return ret;
	}

	/**
	 * Execute the command asynchronously and call the callback actionPerformed
	 * method when the execution ends.
	 * 
	 * The ActionEvent send to the callback actionPerformed method is composed
	 * of :
	 * <ul>
	 * <li>source: The XcosInterpreterManagement.class value which is unique.</li>
	 * <li>id: the command.hashCode() value use it to compare to your own.</li>
	 * <li>command: the command passed to the interpreter.</li>
	 * </ul>
	 * 
	 * Note: the callback is called on the EDT thread so your are free to
	 * manipulate AWT/SWING objects on it.
	 * 
	 * @param command
	 *            The command to execute
	 * @param callback
	 *            The callback which is called at the end of the execution.
	 */
	public static void AsynchronousScilabExec(final String command,
			final ActionListener callback) {
		final int uid = command.hashCode();
		final String uidDesc = Integer.toString(uid);
		final String fullCommand = command + ";xcosNotify(\"" + uidDesc + "\");";
		final ActionEvent event = new ActionEvent(
				XcosInterpreterManagement.class, uid, command);

		executor.execute(new Runnable() {
			public void run() {
				InterpreterManagement.putCommandInScilabQueue(fullCommand);
				Signal.wait(Integer.toString(uid));
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						callback.actionPerformed(event);
					}
				});
			}
		});
	}
}
