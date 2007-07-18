
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Class for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenu implements Menu {

	private SimpleMenu component;
	
	/**
	 * Constructor
	 */
	protected ScilabMenu() {
		component = ScilabBridge.createMenu();
	}
	
	/**
	 * Creates a Scilab Menu object
	 * @return the created Menu
	 */
	public static Menu createMenu() {
		return new ScilabMenu();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleMenu getAsSimpleMenu() {
		return component;
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
	 */
	public void add(MenuItem newMenuItem) {
		ScilabBridge.add(this, newMenuItem);
	}
	
	/**
	 * Sets the text of a Scilab Menu
	 * @param newText the Text to set to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setText(java.lang.String)
	 */
	public void setText(String newText) {
		ScilabBridge.setText(this, newText);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param mnemonic the mnemonic to add to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#setMnemonic(org.scilab.modules.gui.widget.int)
	 */
	public void setMnemonic(int mnemonic) {
		ScilabBridge.setMnemonic(this, mnemonic);
	}
	
	/**
	 * Add a Separator to the Menu
	 * @see org.scilab.modules.gui.menu.Menu#addSeparator(org.scilab.modules.gui..)
	 */
	public void addSeparator() {
		ScilabBridge.addSeparator(this);
	}

}
