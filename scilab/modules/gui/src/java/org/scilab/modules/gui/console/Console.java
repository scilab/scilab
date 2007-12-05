
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab Console in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Console extends Dockable {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleConsole getAsSimpleConsole();

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
	 * Clears the console (all output view is cleared)
	 */
	void clear();
	
	/**
	 * Clears lines from the end of the output view
	 * @param nbLines the number of lines to be deleted
	 */
	void clear(int nbLines);

	/**
	 * Puts the prompt in the top left corner of the console
	 */
	void toHome();

	/**
	 * Sets the prompt displayed in the console
	 * @param prompt the prompt to be displayed in the console
	 */
	void setPrompt(String prompt);

	/**
	 * Updates Scilab internal variables containing the size of the console
	 * These variables are used to format data before displaying it
	 */
	void scilabLinesUpdate();

	/**
	 * Get the current status of the console
	 * If the prompt view is visible, Scilab is waiting for commands
	 * @return true is Scilab is waiting for commands
	 */
	boolean isWaitingForInput();

}
