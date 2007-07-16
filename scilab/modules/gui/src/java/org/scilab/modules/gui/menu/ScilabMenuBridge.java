
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItemBridge;

/**
 * Bridge for Scilab Menus in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabMenuBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Menu
	 * @return the created Menu
	 */
	public static MenuBridge createMenu() {
		return new SwingScilabMenu();
	}
	
	/**
	 * set a text to a menu
	 * @param menu the Menu which we want to set the text to
	 * @param newText the new text to set to the menu
	 */
	public static void setText(MenuBridge menu, String newText) {
		menu.setText(newText);
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 */
	public static MenuItemBridge add(MenuBridge menu, MenuItemBridge newMenuItem) {
		return menu.add(newMenuItem);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param menu the Menu which we want to set the mnemonic to
	 * @param mnemonic the mnemonic to set to the menu
	 */
	public static void setMnemonic(MenuBridge menu, int mnemonic) {
		menu.setMnemonic(mnemonic);
	}
	
	/**
	 * Add a Separator to a Menu
	 * @param menu the Menu which we want to add the Separator to
	 */
	public static void addSeparator(MenuBridge menu) {
		menu.addSeparator();
	}
	
}
