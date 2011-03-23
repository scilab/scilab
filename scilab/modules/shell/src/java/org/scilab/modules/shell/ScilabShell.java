/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
 * Scilab Shell
  */
public class ScilabShell implements Shell {
	/**
	 * Constructor ScilabShell Class.
	 */
	public ScilabShell() {
	}
	
	/**
	 * Displays a line in the Shell
	 * @param lineToPrint the line to be printed
	 */
	public void printf(String lineToPrint) {
		ScilabShellBridge.printf(lineToPrint);
	}

	/**
	 * Clear the shell
	 */
	public void clear() {
		ScilabShellBridge.clear();
	}

	/**
	 * Get the number of lines displayed
	 * @return the number of lines displayed
	 */
	public int lines() {
		return ScilabShellBridge.lines();
	}

	/**
	 * Reads a line in the Shell
	 * @return the line read
	 */
	public String readLine() {
		return ScilabShellBridge.readLine();
	}

	/**
	 * Put the cursor to the top-left position
	 */
	public void tohome() {
		ScilabShellBridge.tohome();
	}
}
/*--------------------------------------------------------------------------*/

