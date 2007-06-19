
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.menu.Menu;

/**
 * Bridge for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBarBridge {

	/**
	 * Constructor
	 */
	protected ScilabMenuBarBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab MenuBar
	 * @return the created MenuBar
	 */
	public static MenuBar createMenuBar() {
		return new SwingScilabMenuBar();
	}
	
	/**
	 * Append a Menu to a MenuBar
	 * @param menuBar the MenuBar which we want to add the newMenu to
	 * @param newMenu the Menu to add to the MenuBar
	 * @return the added Menu
	 */
	public static Menu add(MenuBar menuBar, Menu newMenu) {
		return menuBar.add(newMenu);
	}
}
