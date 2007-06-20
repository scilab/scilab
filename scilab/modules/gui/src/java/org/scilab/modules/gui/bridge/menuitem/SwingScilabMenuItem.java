
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menuitem;

import javax.swing.JMenuItem;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuItem extends JMenuItem implements MenuItem {

	/**
	 * Constructor
	 */
	public SwingScilabMenuItem() {
		super();
	}
	
	/**
	 * Sets the Text of a swing Scilab MenuItem
	 * @param newText the Text to set to the MenuItem
	 * @see javax.swing.AbstractButton#setText(java.lang.String)
	 */
	public void setText(String newText) {
		super.setText(newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic to add to the MenuItem
	 * @see org.scilab.modules.gui.widget.MenuItem#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic);
	}
}
