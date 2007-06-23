
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.Window;

/**
 * Bridge for Scilab MenuBars in GUIs
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
	public static Menu createMenu() {
		return new SwingScilabMenu();
	}
	
	/**
	 * Sets the dimensions (width and height) of a swing Scilab Menu
	 * @param menu the Menu which we want to set the newSize to
	 * @param newSize the dimensions to set to the Menu
	 */
	public static void setDims(Menu menu, Size newSize) {
		menu.setDims(newSize);
	}
	
	/**
	 * Gets the dimensions (width and height) of a Scilab menu
	 * @param menu the menu we want to get the dimensions of
	 * @return the dimensions of the menu
	 */
	public static Size getDims(Menu menu) {
		return menu.getDims();
	}
	
	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab menu
	 * @param menu the menu we want to get the position of
	 * @return the position of the menu
	 */
	public static Position getPosition(Menu menu) {
		return menu.getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab menu
	 * @param menu the menu we want to set the position of
	 * @param newPosition the position to set to the menu
	 */
	public static void setPosition(Menu menu, Position newPosition) {
		menu.setPosition(newPosition);
	}
	
	/**
	 * Gets the visibility status of a Scilab menu
	 * @param menu the menu we want to getthe visibility status of
	 * @return the visibility status of the window (true if the menu is visible, false if not)
	 */
	public static boolean isVisible(Menu menu) {
		return menu.isVisible();
	}

	/**
	 * Sets the visibility status of a Scilab menu
	 * @param menu the menu we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the menu (true to set the menu visible, false else)
	 */
	public static void setVisible(Menu menu, boolean newVisibleState) {
		menu.setVisible(newVisibleState);
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 * @return the added MenuItem
	 */
	public static MenuItem add(Menu menu, MenuItem newMenuItem) {
		return menu.add(newMenuItem);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param menu the Menu which we want to set the mnemonic to
	 * @param mnemonic the mnemonic to set to the menu
	 */
	public static void setMnemonic(Menu menu, int mnemonic) {
		menu.setMnemonic(mnemonic);
	}
	
	/**
	 * Add a Separator to a Menu
	 * @param menu the Menu which we want to add the Separator to
	 */
	public static void addSeparator(Menu menu) {
		menu.addSeparator();
	}
	
}
