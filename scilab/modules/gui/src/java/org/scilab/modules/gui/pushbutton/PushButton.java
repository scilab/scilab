
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for PushButton associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface PushButton extends Widget {

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimplePushButton getAsSimplePushButton();

	/**
	 * Sets the icon of a PushButton
	 * @param filename the path to the icon image to set to the PushButton
	 */
	void setIcon(String filename);
}
