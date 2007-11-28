
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menuitem;

import javax.swing.JMenuItem;

import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.gui.utils.SciActionListener;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuItem extends JMenuItem implements SimpleMenuItem {

	private static final long serialVersionUID = 1L;

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
	@Override
	public void setText(String newText) {
		super.setText(newText);
	}
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic to add to the MenuItem
	 * @see org.scilab.modules.gui.widget.MenuItem#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	@Override
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic);
	}
	
	/**
	 * Add a callback to the menu, this callback is a Scilab command
	 * @param command the Scilab command to execute when the menu is activated
	 */
	public void setCallback(String command) {
		super.addActionListener(new SciActionListener(command));
	}
}
