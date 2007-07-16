
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menubar;

import javax.swing.JMenuBar;
import javax.swing.JMenu;

import org.scilab.modules.gui.menu.MenuBridge;
import org.scilab.modules.gui.menubar.MenuBarBridge;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuBar extends JMenuBar implements MenuBarBridge {

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
	public MenuBridge add(MenuBridge newMenu) {
		return (MenuBridge) super.add((JMenu) newMenu);
	}
}
