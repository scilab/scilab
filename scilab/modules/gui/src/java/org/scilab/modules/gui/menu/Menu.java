
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Interface for MenuBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Menu extends MenuItem {
	//	 TODO : Add the methods that are usefull for a Menu

	/**
	 * Append a MenuItem to the Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 */
	MenuItem add(MenuItem newMenuItem);
}
