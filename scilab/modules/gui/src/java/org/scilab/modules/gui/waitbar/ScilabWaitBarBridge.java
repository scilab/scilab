/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.waitbar;

import org.scilab.modules.gui.bridge.waitbar.SwingScilabWaitBar;

/**
 * Bridge dedicated to Scilab Wait Bars
 * @author Vincent COUVERT
 */
public class ScilabWaitBarBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabWaitBarBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Create a new WaitBar
	 * @return the created WaitBar
	 */
	public static SimpleWaitBar createWaitBar() {
		return new SwingScilabWaitBar();
	}

	/**
	 * Set the title of the WaitBar
	 * @param waitBar the WaitBar
	 * @param title the title to set
	 */
	public static void setTitle(WaitBar waitBar, String title) {
		waitBar.getAsSimpleWaitBar().setTitle(title);
	}
	
	/**
	 * Set the message of the WaitBar
	 * @param waitBar the WaitBar
	 * @param message the message to set
	 */
	public static void setMessage(WaitBar waitBar, String[] message) {
		waitBar.getAsSimpleWaitBar().setMessage(message);
	}
	
	/**
	 * Set the current value of the WaitBar
	 * @param waitBar the WaitBar
	 * @param value the value to set
	 */
	public static void setValue(WaitBar waitBar, int value) {
		waitBar.getAsSimpleWaitBar().setValue(value);
	};
	
	/**
	 * Close the WaitBar
	 * @param waitBar the WaitBar
	 */
	public static void close(WaitBar waitBar) {
		waitBar.getAsSimpleWaitBar().close();
	}
	
	/**
	 * Indicates if the total execution time is known
	 * @param waitBar the WaitBar
	 * @param status true if the total progress time in unknown
	 */
	public static void setIndeterminateMode(WaitBar waitBar, boolean status) {
		waitBar.getAsSimpleWaitBar().setIndeterminateMode(status);
	}

}
