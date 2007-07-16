
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.menu.SimpleMenu;

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
	public static SimpleMenuBar createMenuBar() {
		return new SwingScilabMenuBar();
	}
	
	/**
	 * Append a Menu to a MenuBar
	 * @param menuBar the MenuBar which we want to add the newMenu to
	 * @param newMenu the Menu to add to the MenuBar
	 */
	// * @return the added Menu
	public static void add(SimpleMenuBar menuBar, SimpleMenu newMenu) {
		menuBar.add(newMenu);
	}
}
