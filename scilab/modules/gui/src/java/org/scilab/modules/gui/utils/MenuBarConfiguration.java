
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;

/**
 * Scilab menubar configuration
 * All methods needed to create a menubar
 * @author Vincent COUVERT
 */
public interface MenuBarConfiguration {

	/**
	 * Add menus to a menubar
	 * @param mb the menubar which the menus will be added to
	 */
	void addMenus(MenuBar mb);
	
	/**
	 * Read submenus data in the XML file
	 * @param menu the parent menu for submenus
	 * @param index the index of the parent in menu list
	 */
	void addSubMenus(Menu menu, int index);
}
