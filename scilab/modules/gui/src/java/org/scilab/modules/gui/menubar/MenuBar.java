
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for MenuBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface MenuBar extends UIElement {
	//	 TODO : Add the methods that are usefull for a MenuBar
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenuBar getAsSimpleMenuBar();
	
	/**
	 * Add a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 */
	void add(Menu newMenu);
}
