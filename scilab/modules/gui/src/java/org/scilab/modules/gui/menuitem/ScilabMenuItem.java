
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Class for Scilab MenuItem in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuItem implements MenuItem {

	private SimpleMenuItem component;
	
	/**
	 * Constructor
	 */
	protected ScilabMenuItem() {
        component = ScilabBridge.createMenuItem();
	}

	/**
	 * Creates a Scilab MenuItem object
	 * @return the created MenuItem
	 */
	public static MenuItem createMenuItem() {
		return new ScilabMenuItem();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleMenuItem getAsSimpleMenuItem() {
		return component;
	}
	
	/**
	 * Sets the text of a Scilab MenuItem
	 * @param newText the Text to set to the MenuItem
	 * @see org.scilab.modules.gui.widget.MenuItem#setText(java.lang.String)
	 */
	public void setText(String newText) {
		ScilabBridge.setText(this, newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic to add to the MenuItem
	 * @see org.scilab.modules.gui.widget.MenuItem#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(char mnemonic) {
		ScilabBridge.setMnemonic(this, mnemonic);
	}

	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 */
	public void setCallback(String command) {
		ScilabBridge.setCallback(this, command);
	}
	
}
