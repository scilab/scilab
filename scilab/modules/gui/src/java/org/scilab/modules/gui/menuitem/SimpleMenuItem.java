
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimpleMenuItem the associated object to Scilab GUIs MenuItem
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuItem extends Widget {
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic for  the MenuItem
	 */
	void setMnemonic(char mnemonic);

	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic for  the MenuItem
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 * @param commandType the type of the command that will be executed.
	 */
	void setCallback(String command, int commandType);

	/**
	 * Set if the menu item is enabled or not
	 * @param status true if the menu item is enabled
	 */
	void setEnabled(boolean status);
	
	/**
	 * Add a MenuItem to this MenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	void add(MenuItem childMenuItem);

	/**
	 * Destroy the MenuItem
	 */
	void destroy();

}
