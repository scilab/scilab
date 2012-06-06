/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.utils;

/**
 * Class use for dialogs that wait for a user input
 * 
 * @author Bruno JOFRET
 */
public final class StringBlockingResult extends BlockingResult<String> {

	private static StringBlockingResult me;
	
	/**
	 * Get the current instance of BlockingResult
	 * @return this instance
	 */
	public static StringBlockingResult getInstance() {
		if (me == null) {
			me = new StringBlockingResult();	
		}
		return me;
	}
}
