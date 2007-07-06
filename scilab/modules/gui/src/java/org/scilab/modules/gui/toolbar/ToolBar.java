
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Interface for ToolBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface ToolBar {
	// TODO : Add the methods that are usefull for a ToolBar
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	void add(PushButton pushButton);
}
