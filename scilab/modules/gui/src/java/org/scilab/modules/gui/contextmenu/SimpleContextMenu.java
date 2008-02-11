
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.contextmenu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;

/**
 * Interface for SimpleMenu the associated object to Scilab GUIs Menu
 * @author Vincent COUVERT
 */
public interface SimpleContextMenu extends SimpleMenuItem {
	
	/**
	 * Append a MenuItem to the ContextMenu
	 * @param newMenuItem the MenuItem to add to the ContextMenu
	 */
	void add(MenuItem newMenuItem);
	
	/**
	 * Display the ContextMenu
	 */
	void display();
}
