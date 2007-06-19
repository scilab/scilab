
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;

/**
 * Interface for MenuBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface MenuBar {
	//	 TODO : Add the methods that are usefull for a MenuBar
	
	/**
	 * Add a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @return the added Menu
	 */
	Menu add(Menu newMenu);
}
