
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

	/**
	 * Set the element id for this menubar
	 * @param id the id of the corresponding menubar object
	 */
	void setElementId(int id);
	
	/**
	 * Get the element id for this menubar
	 * @return id the id of the corresponding menubar object
	 */
	int getElementId();
	
	/**
	 * Enable/Disable a menu giving its name
	 * @param menuName the name of the menu
	 * @param status true to enable the menu
	 */
	void setMenuEnabled(String menuName, boolean status);
	
	/**
	 * Disable a MenuItem of a Scilab root window giving its parent name and position
	 * @param parentMenuName the name of the parent menu
	 * @param menuItemPosition the name of the parent menu
	 * @param status true to set the menu enabled
	 */
	void setSubMenuEnabled(String parentMenuName, int menuItemPosition, boolean status);
	
	/**
	 * Remove a menu giving its name
	 * @param menuName the name of the menu
	 */
	void removeMenu(String menuName);
}
