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
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Implements useful methods to communicate with the Scilab interpreter.
 */
public final class XcosInterpreterManagement extends InterpreterManagement {

	private static ExecutorService executor = Executors.newSingleThreadExecutor();
	private static Set<String> runningTasks = Collections.synchronizedSet(new HashSet<String>());
	
	private static final String NOTIFY = ";xcosNotify(\"";
	private static final String CLOSE = "\");";

	/** This class is a static singleton, thus it must not be instantiated */
	private XcosInterpreterManagement() { }
	
	/**
	 * Throw when there is a problem to communicate with the scilab interpreter.
	 */
	public static class InterpreterException extends Exception {
		/**
		 * Default constructor
		 * @param string Useful message 
		 */
		public InterpreterException(String string) {
			super(string);
		}
	}

	/**
	 * Execute the command and wait for the end of the execution.
	 * 
	 * @param command
	 *            The scilab command
	 * @throws InterpreterException when the command cannot be executed on the interpreter.
	 */
	public static void synchronousScilabExec(String command) throws InterpreterException {
		final String uidDesc = Integer.toString(command.hashCode());
		final String fullCommand = command + NOTIFY + uidDesc + CLOSE;
		
		if (runningTasks.contains(uidDesc)) {
			throw new InterpreterException(XcosMessages.SCILAB_SAMECOMMAND);
		}
		
		int ret = InterpreterManagement.requestScilabExec(fullCommand);
		if (ret != 0) {
			throw new InterpreterException(XcosMessages.SCILAB_UNABLE);
		}
		runningTasks.add(uidDesc);
		Signal.wait(uidDesc);
		runningTasks.remove(uidDesc);
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
	 * @throws InterpreterException when the command cannot be executed on the interpreter.
	 */
	public static void asynchronousScilabExec(String command,
			final ActionListener callback) throws InterpreterException {
		final int uid = command.hashCode();
		final String uidDesc = Integer.toString(uid);
		final String fullCommand = command + NOTIFY + uidDesc + CLOSE;
		final ActionEvent event = new ActionEvent(
				XcosInterpreterManagement.class, uid, command);

		if (runningTasks.contains(uidDesc)) {
			throw new InterpreterException(XcosMessages.SCILAB_SAMECOMMAND);
		}
		
		executor.submit(new Callable<Void>() {
			public Void call() throws Exception {
				int ret = InterpreterManagement.putCommandInScilabQueue(fullCommand);
				if (ret != 0) {
					throw new InterpreterException(XcosMessages.SCILAB_UNABLE);
				}
				runningTasks.add(uidDesc);
				Signal.wait(uidDesc);
				runningTasks.remove(uidDesc);
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						callback.actionPerformed(event);
					}
				});
				return null;
			}
		});
	}
	
	/**
	 * This method halt a command performed asynchronously.
	 * @param hashcode The command.getHashcode() uid.
	 */
	public static void stopScilabExec(int hashcode) {
		String uidDesc = Integer.toString(hashcode);
		
		synchronized (runningTasks) {
			if (runningTasks.contains(uidDesc)) {
				Signal.notify(uidDesc);
				runningTasks.remove(uidDesc);
			}
		}
	}
	
	/** 
	 * This method stop all the running scilab execution (sync or async).
	 */
	public static void stopAllScilabExec() {
		synchronized (runningTasks) {
			interruptScilab();
			for (String uidDesc : runningTasks) {
				Signal.notify(uidDesc);
			}
		}
	}
}
