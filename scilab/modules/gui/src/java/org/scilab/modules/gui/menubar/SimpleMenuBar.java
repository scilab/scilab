
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.SimpleMenu;

/**
 * Interface for MenuBarBridge the associated object to Scilab GUIs MenuBar
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuBar {
	
	/**
	 * Add a MenuBridge to a MenuBarBridge
	 * @param newMenu the MenuBridge to add to the MenuBarBridge
	 * @return the added MenuBridge
	 */
	SimpleMenu add(SimpleMenu newMenu);
}
