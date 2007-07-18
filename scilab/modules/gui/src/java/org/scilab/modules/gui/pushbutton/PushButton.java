
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
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	void setIcon(String filename);
}
