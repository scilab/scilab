
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimpleMenu the associated object to Scilab GUIs Menu
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenu extends Widget {
	
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
	 * @return the text of the Menu
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
