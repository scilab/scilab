
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;

/**
 * Interface for SimpleMenuBar the associated object to Scilab GUIs MenuBar
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuBar {
	
	/**
	 * Add a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 */
	void add(Menu newMenu);
	
}
