
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.popupmenu;

import javax.swing.JPopupMenu;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.popupmenu.PopupMenu;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
* Swing implementation for Scilab PopupMenu in GUIs
* @author Marouane BEN JELLOUL
*/
public class SwingScilabPopupMenu extends JPopupMenu implements PopupMenu {
	
	/**
	 * Append a MenuItem to the PopupMenu
	 * @param newMenuItem the MenuItem to add to the PopupMenu
	 * @return the added MenuItem
	 */
	public MenuItem add(MenuItem newMenuItem) {
		return (MenuItem) super.add((SwingScilabMenuItem) newMenuItem);
	}
	
	/**
	 * Displays the popup menu at the position x,y in the coordinate space of the component invoker.
	 * @param pushButton - the component in whose space the popup menu is to appear
	 * @param x - the x coordinate in invoker's coordinate space at which the popup menu is to be displayed
	 * @param y - the y coordinate in invoker's coordinate space at which the popup menu is to be displayed
	 */
	public void show(PushButton pushButton, int x, int y) {
		super.show((SwingScilabPushButton) pushButton, x, y);
	}


}
