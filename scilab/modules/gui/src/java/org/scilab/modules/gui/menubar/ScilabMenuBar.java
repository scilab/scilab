
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menu.Menu;

/**
 * Class for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBar implements MenuBar {

	private SimpleMenuBar component;
	
	/**
	 * Constructor
	 */
	protected ScilabMenuBar() {
		component = ScilabBridge.createMenuBar();
	}

	/**
	 * Creates a Scilab MenuBar object
	 * @return the created MenuBar
	 */
	public static MenuBar createMenuBar() {
		return new ScilabMenuBar();
	}
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleMenuBar getAsSimpleMenuBar() {
		return component;
	}
	
	/**
	 * Add a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.Menu)
	 */
//	* @return the added Menu
	public void add(Menu newMenu) {
		ScilabBridge.add(component, newMenu.getAsSimpleMenu());
	}
}
