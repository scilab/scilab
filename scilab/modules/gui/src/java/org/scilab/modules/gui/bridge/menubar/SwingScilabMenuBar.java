
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menubar;

import javax.swing.JMenuBar;
import javax.swing.JMenu;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.SimpleMenuBar;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuBar extends JMenuBar implements SimpleMenuBar {

	/**
	 * Constructor
	 */
	public SwingScilabMenuBar() {
		super();
	}

	/**
	 * Append a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.Menu)
	 */
	public void add(Menu newMenu) {
		super.add((JMenu) newMenu.getAsSimpleMenu());
	}
	
}
