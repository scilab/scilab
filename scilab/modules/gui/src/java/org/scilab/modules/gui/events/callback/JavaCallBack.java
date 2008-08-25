/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.events.callback;

import java.lang.reflect.Method;

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
}
