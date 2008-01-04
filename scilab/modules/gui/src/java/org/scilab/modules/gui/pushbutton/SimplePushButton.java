
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimplePushButton the associated object to Scilab GUIs PushButton
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public interface SimplePushButton extends Widget {

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	void setIcon(String filename);
	
	/**
	 * Add a callback to the pushbutton, this callback is a Scilab command
	 * @param command the Scilab command to execute when the pushbutton is activated
	 * @param commandType the type of the command that will be executed.
	 */
	void setCallback(String command, int commandType);
}
