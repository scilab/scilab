
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.widget.Widget;

/**
* Interface for PushButton associated to objects in Scilab GUIs
* @author Marouane BEN JELLOUL
*/
public interface PushButton extends Widget {
	// TODO : Add the methods that are usefull for a PushButton
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimplePushButton getAsSimplePushButton();
	
	/**
	 * Sets the text of a PushButton
	 * @param text the text to set to the PushButton
	 */
	void setText(String text);

	/**
	 * Gets the text of a PushButton
	 * @return the text of the PushButton
	 */
	String getText();

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

	/**
	 * Set if the pushbutton is enabled or not
	 * @param status true if the pushbutton is enabled
	 */
	void setEnabled(boolean status);
}
