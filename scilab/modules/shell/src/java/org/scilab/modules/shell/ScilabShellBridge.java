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
 * Scilab Bridge for Scilab Shell object
 *
 */
public class ScilabShellBridge {

	/**
	 * Constructor for ScilabShellBridge class
	 */
	protected ScilabShellBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Displays a line in the Shell
	 * @param lineToPrint the line to be printed
	 */
	public static void printf(String lineToPrint) {
		// TODO Auto-generated method stub
	}

	/**
	 * Clear the shell
	 */
	public static void clear() {
		// TODO Auto-generated method stub
	}

	/**
	 * Get the number of lines displayed
	 * @return the number of lines displayed
	 */
	public static int lines() {
		// TODO Auto-generated method stub
		return 0;
	}

	/**
	 * Reads a line in the Shell
	 * @return the line read
	 */
	public static String readLine() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Put the cursor to the top-left position
	 */
	public static void tohome() {
		// TODO Auto-generated method stub
		
	};

}
