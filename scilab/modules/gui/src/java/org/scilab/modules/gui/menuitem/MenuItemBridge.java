
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

/**
 * Interface for MenuItemBridge the associated object to Scilab GUIs MenuItem
 * @author Marouane BEN JELLOUL
 */
public interface MenuItemBridge {
	
	/**
	 * Sets the text of a MenuItemBridge
	 * @param newText the text we want to set
	 */
	void setText(String newText);
	
	/**
	 * set a mnemonic to a MenuItemBridge
	 * @param mnemonic the int to add to the MenuItemBridge
	 */
	void setMnemonic(int mnemonic);
	
}
