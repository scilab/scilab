package org.scilab.modules.shell;
/* Copyright INRIA 2007 */

/**
 * Interface for Scilab shell
 * @author Vincent COUVERT
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

