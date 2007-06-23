
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menu.Menu;

/**
 * Class for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBar implements MenuBar {

	/**
	 * Constructor
	 */
	protected ScilabMenuBar() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab MenuBar object
	 * @return the created MenuBar
	 */
	public static MenuBar createMenuBar() {
		return ScilabBridge.createMenuBar();
	}
	
	/**
	 * Add a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @return the added Menu
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.Menu)
	 */
	public Menu add(Menu newMenu) {
		return ScilabBridge.add(this, newMenu);
	}
}
