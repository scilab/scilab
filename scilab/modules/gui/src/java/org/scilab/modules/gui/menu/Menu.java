
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for Menu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Menu extends Widget {
	//	 TODO : Add the methods that are usefull for a Menu

	/**
	 * Append a MenuItem to the Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 */
	MenuItem add(MenuItem newMenuItem);
	
	/**
	 * Sets the text of the Menu
	 * @param newText the text we want to set for the Menu
	 */
	void setText(String newText);
	
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
