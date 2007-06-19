
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenu extends ScilabMenuItem implements Menu {

	/**
	 * Constructor
	 */
	protected ScilabMenu() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Menu object
	 * @return the created Menu
	 */
	public static Menu createMenu() {
		return ScilabBridge.createMenu();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 * @see org.scilab.modules.gui.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return ScilabBridge.add(this, newMenuItem);
	}
}
