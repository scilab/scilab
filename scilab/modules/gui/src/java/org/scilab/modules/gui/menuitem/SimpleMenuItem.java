
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

/**
 * Interface for SimpleMenuItem the associated object to Scilab GUIs MenuItem
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuItem {
	
	/**
	 * Sets the text of a MenuItem
	 * @param newText the text we want to set
	 */
	void setText(String newText);
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the int to add to the MenuItem
	 */
	void setMnemonic(char mnemonic);
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 */
	void setCallback(String command);

	/**
	 * Set if the menu item is enabled or not
	 * @param status true if the menu item is enabled
	 */
	void setEnabled(boolean status);
}
