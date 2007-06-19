
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menu;

import javax.swing.JMenu;
import javax.swing.JMenuItem;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenu extends JMenu implements Menu {

	/**
	 * Constructor
	 */
	public SwingScilabMenu() {
		super();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 * @see org.scilab.modules.gui.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return (MenuItem) super.add((JMenuItem) newMenuItem);
	}
	
}
