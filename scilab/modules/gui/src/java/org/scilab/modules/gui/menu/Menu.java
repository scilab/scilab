
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for Menu associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface Menu extends Widget {
	//	 TODO : Add the methods that are usefull for a Menu
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenu getAsSimpleMenu();
	
	/**
	 * Append a MenuItem to the Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 */
	void add(MenuItem newMenuItem);
	
	/**
	 * Append a subMenu to the Menu
	 * @param newSubMenu the subMenu to append to the Menu
	 */
	void add(Menu newSubMenu);
	
	/**
	 * Sets the text of the Menu
	 * @param newText the text we want to set for the Menu
	 */
	void setText(String newText);
	
	/**
	 * Gets the text of the Menu
	 * @return the text we want to set for the Menu
	 */
	String getText();
	
	/**
	 * set a mnemonic to the Menu
	 * @param mnemonic the new mnemonic of the Menu
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Add a Separator to a Menu
	 */
	void addSeparator();
	
}
