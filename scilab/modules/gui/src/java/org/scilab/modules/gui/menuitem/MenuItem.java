
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for MenuItem associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface MenuItem extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenuItem getAsSimpleMenuItem();
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the Mnemonic of the MenuItem
	 */
	void setMnemonic(char mnemonic);
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the Mnemonic of the MenuItem
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Set if the menu item is enabled or not
	 * @param status true if the menu item is enabled
	 */
	void setEnabled(boolean status);
	
	/**
	 * Add a Menu to this MenuItem
	 * @param childMenu the Menu we want to add
	 */
	void add(Menu childMenu);

	/**
	 * Add a MenuItem to this MenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	void add(MenuItem childMenuItem);
}
