/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.events;

/**
 * Class use for dialogs that wait for a user input
 * 
 * @author Bruno JOFRET
 */
public final class BlockingResult {

	private static BlockingResult me;
	
	private String theResult;
	
	/**
	 * Constructor
	 */
	private BlockingResult() {
		theResult = null;
	}

	/**
	 * Get the current instance of BlockingResult
	 * @return this instance
	 */
	public static BlockingResult getInstance() {
		if (me == null) {
			me = new BlockingResult();	
		}
		return me;
	}
	
	/**
	 * Get the user input (wait until it)
	 * @return the user input
	 */
	public String getResult() {
		synchronized (me) {
			try {
				me.wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return me.theResult;
	}

	/**
	 * Set the result for this BlockingResult and notify
	 * @param theResult the user input to set
	 */
	public void setResult(String theResult) {
		this.theResult = theResult;
		synchronized (me) {
			me.notify();
		}
	}
	
	
	
}
