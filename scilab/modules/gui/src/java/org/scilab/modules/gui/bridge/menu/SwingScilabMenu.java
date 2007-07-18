
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menu;

import javax.swing.JMenu;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenu extends JMenu implements SimpleMenu {

	/**
	 * Constructor
	 */
	public SwingScilabMenu() {
		super();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public void add(MenuItem newMenuItem) {
		super.add((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem());
	}
	
	/**
	 * Sets the Text of a swing Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see javax.swing.AbstractButton#setText(java.lang.String)
	 */
	@Override
	public void setText(String newText) {
		super.setText(newText);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	@Override
	public void setMnemonic(int mnemonic) {
		super.setMnemonic(mnemonic);
	}

	/**
	 * Add a Separator to a Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	@Override
	public void addSeparator() {
		// TODO correct this it should be something like super.add(SwingScilabSeparator) ... ???
		super.addSeparator();
	}

}
