
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
	public static MenuItem createMenuItem() {
		return new SwingScilabMenuItem();
	}
	
	/**
	 * Sets the Text of a Scilab MenuItem
	 * @param menuItem the MenuItem we want to set the Text of
	 * @param newText the Text we want to set to the menuItem
	 */
	public static void setText(MenuItem menuItem, String newText) {
		menuItem.setText(newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param menuItem the MenuItem which we want to add the mnemonic to
	 * @param mnemonic the mnemonic to add to the MenuItem
	 */
	public static void setMnemonic(MenuItem menuItem, int mnemonic) {
		menuItem.setMnemonic(mnemonic);
	}
}
