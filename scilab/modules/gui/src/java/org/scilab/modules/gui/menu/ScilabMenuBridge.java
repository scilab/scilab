
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;

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
	public static SimpleMenu createMenu() {
		return new SwingScilabMenu();
	}
	
	/**
	 * set a text to a menu
	 * @param menu the Menu which we want to set the text to
	 * @param newText the new text to set to the menu
	 */
	public static void setText(Menu menu, String newText) {
		String label = newText;
		
		// Try to set a mnemonic according to text (character preeceded by a &)
		for (int charIndex = 0; charIndex < newText.length(); charIndex++) {
			if (newText.charAt(charIndex) == '&') {
				
				boolean canBeAMnemonic = true;
				
				// Previous char must not be a &
				if ((charIndex != 0) && (newText.charAt(charIndex - 1) == '&')) {
					canBeAMnemonic = false;
				}

				if (canBeAMnemonic && newText.charAt(charIndex + 1) != '&') {
					// A mnemonic
					menu.getAsSimpleMenu().setMnemonic(newText.charAt(charIndex + 1));
					
					// Have to remove the & used to set a Mnemonic
					String firstPart = newText.substring(0, Math.max(charIndex - 1, 0)); // Before &
					String secondPart = newText.substring(Math.min(charIndex + 1, newText.length()), newText.length()); // After &
					label = firstPart + secondPart; 
					break;
				}

			}
		}

		// Set the text after relacing all && (display a & in the label) by &
		menu.getAsSimpleMenu().setText(label.replaceAll("&&", "&"));
	}
	
	/**
	 * Get a text of a menu
	 * @param menu the Menu which we want to set the text to
	 * @return the text of the menu
	 */
	public static String getText(Menu menu) {
		return menu.getAsSimpleMenu().getText();
	}
	
	/**
	 * Append a MenuItem to a Scilab Menu
	 * @param menu the Menu which we want to add the MenuItem to
	 * @param newMenuItem the MenuItem to add to the Menu
	 */
	public static void add(Menu menu, MenuItem newMenuItem) {
		menu.getAsSimpleMenu().add(newMenuItem);
	}
	
	/**
	 * Append a subMenu to a Scilab Menu
	 * @param menu the Menu which we want to append the subMenu to
	 * @param newSubMenu the subMenu to append to the Menu
	 */
	public static void add(Menu menu, Menu newSubMenu) {
		menu.getAsSimpleMenu().add(newSubMenu);
	}
	
	/**
	 * set a mnemonic to a Menu
	 * @param menu the Menu which we want to set the mnemonic to
	 * @param mnemonic the mnemonic to set to the menu
	 */
	public static void setMnemonic(Menu menu, int mnemonic) {
		menu.getAsSimpleMenu().setMnemonic(mnemonic);
	}
	
	/**
	 * Add a Separator to a Menu
	 * @param menu the Menu which we want to add the Separator to
	 */
	public static void addSeparator(Menu menu) {
		menu.getAsSimpleMenu().addSeparator();
	}
	
}
