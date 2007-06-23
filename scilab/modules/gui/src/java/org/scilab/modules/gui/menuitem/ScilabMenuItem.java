
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
 * Class for Scilab MenuItem in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuItem implements MenuItem {

	/**
	 * Constructor
	 */
	protected ScilabMenuItem() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab MenuItem object
	 * @return the created MenuItem
	 */
	public static MenuItem createMenuItem() {
		return ScilabBridge.createMenuItem();
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
	public void setMnemonic(int mnemonic) {
		ScilabBridge.setMnemonic(this, mnemonic);
	}
}
