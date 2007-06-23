
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

/**
 * Interface for MenuItem associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface MenuItem {
	
	/**
	 * Sets the text of a MenuItem
	 * @param newText the text we want to set for the MenuItem
	 */
	void setText(String newText);
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the int to add to the MenuItem
	 */
	void setMnemonic(int mnemonic);
	
	//	 TODO : Add the methods that are usefull for a MenuItem
}
