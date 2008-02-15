
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.contextmenu;

import org.scilab.modules.gui.menu.Menu;
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
	 * Append a Menu to the ContextMenu
	 * @param newMenu the Menu to add to the ContextMenu
	 */
	void add(Menu newMenu);

	/**
	 * Display the ContextMenu
	 * @return the label of the menu pressed
	 */
	String displayAndWait();
}
