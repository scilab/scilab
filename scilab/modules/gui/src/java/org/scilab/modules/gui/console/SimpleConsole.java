
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.dockable.SimpleDockable;

/**
 * Interface for SimpleConsole the associated object to Scilab GUIs Console
 * @author Marouane BEN JELLOUL
 */
public interface SimpleConsole extends SimpleDockable {

	/**
	 * Reads input data in console
	 * @return the string entered by user
	 */
	String readLine();

	/**
	 * Displays data in Console
	 * @param dataToDisplay the data to be displayed
	 */
	void display(String dataToDisplay);
	
	/**
	 * Clears the console (no more scroll history and prompt on top left corner)
	 */
	void clear();
}
