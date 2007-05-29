
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.console;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab Console in GUIs
 * @author Vincent COUVERT
 *
 */
public interface Console extends Dockable {

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
}
