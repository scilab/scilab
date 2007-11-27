
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

/**
 * Interface for MenuItem associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface MenuItem {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenuItem getAsSimpleMenuItem();
	
	/**
	 * Sets the text of a MenuItem
	 * @param newText the text we want to set for the MenuItem
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
}
