
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for ToolBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface ToolBar extends UIElement {
	// TODO : Add the methods that are usefull for a ToolBar
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleToolBar getAsSimpleToolBar();
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	void add(PushButton pushButton);

	/**
	 * Add a Separator to a toolbar
	 */
	void addSeparator();
}
