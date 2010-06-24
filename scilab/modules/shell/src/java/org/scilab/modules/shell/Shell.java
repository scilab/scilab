/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.shell;

/**
 * Interface for Scilab shell
 *
 */
public interface Shell {
	
	/**
	 * Displays a line in the Shell
	 * @param lineToPrint the line to be printed
	 */
	void printf(String lineToPrint);
	
	/**
	 * Reads a line in the Shell
	 * @return the line read
	 */
	String readLine();
	
	/**
	 * Clear the shell
	 */
	void clear();
	
	/**
	 * Put the cursor to the top-left position
	 */
	void tohome();
	
	/**
	 * Get the number of lines displayed
	 * @return the number of lines displayed
	 */
	int lines();

}
/*--------------------------------------------------------------------------*/

