
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge;

import javax.swing.JMenuBar;
import javax.swing.JMenu;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuBar extends JMenuBar implements MenuBar {

	/**
	 * Constructor
	 */
	public SwingScilabMenuBar() {
		super();
	}

	/**
	 * Append a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @return the added Menu
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.Menu)
	 */
	public Menu add(Menu newMenu) {
		return (Menu) super.add((JMenu) newMenu);
	}
}
