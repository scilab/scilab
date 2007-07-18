
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
	void setMnemonic(int mnemonic);
	
}
