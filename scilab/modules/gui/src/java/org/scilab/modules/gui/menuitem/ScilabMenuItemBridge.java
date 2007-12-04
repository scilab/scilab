
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;

/**
 * Bridge for Scilab MenuItem in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuItemBridge {

	/**
	 * Constructor
	 */
	protected ScilabMenuItemBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab MenuItem
	 * @return the created MenuItem
	 */
	public static SimpleMenuItem createMenuItem() {
		return new SwingScilabMenuItem();
	}
	
	/**
	 * Sets the Text of a Scilab MenuItem
	 * @param menuItem the MenuItem we want to set the Text of
	 * @param newText the Text we want to set to the menuItem
	 */
	public static void setText(MenuItem menuItem, String newText) {
		menuItem.getAsSimpleMenuItem().setText(newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param mnemonic the mnemonic to add to the MenuItem
	 */
	public static void setMnemonic(MenuItem menuItem, char mnemonic) {
		menuItem.getAsSimpleMenuItem().setMnemonic(mnemonic);
	}
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param command the Scilab command to execute when the menu is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public static void setCallback(MenuItem menuItem, String command, int commandType) {
		menuItem.getAsSimpleMenuItem().setCallback(command, commandType);
	}

	/**
	 * Set if the menu item is enabled or not
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param status true if the menu item is enabled
	 */
	public static void setEnabled(MenuItem menuItem, boolean status) {
		menuItem.getAsSimpleMenuItem().setEnabled(status);
	}
	
}
