
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Interface for PopupMenu associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface PopupMenu {
	// TODO : Add the methods that are usefull for a PopupMenu
	
	/**
	 * Append a MenuItem to the PopupMenu
	 * @param newMenuItem the MenuItem to add to the PopupMenu
	 * @return the added MenuItem
	 */
	MenuItem add(MenuItem newMenuItem);
	
	/**
	 * Displays the popup menu at the position x,y in the coordinate space of the component invoker.
	 * @param pushButton - the component in whose space the popup menu is to appear
	 * @param x - the x coordinate in invoker's coordinate space at which the popup menu is to be displayed
	 * @param y - the y coordinate in invoker's coordinate space at which the popup menu is to be displayed
	 */
	void show(PushButton pushButton, int x, int y);

}
