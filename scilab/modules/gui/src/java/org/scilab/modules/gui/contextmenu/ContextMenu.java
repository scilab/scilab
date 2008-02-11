
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.contextmenu;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for ContextMenu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface ContextMenu extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleContextMenu getAsSimpleContextMenu();
	
	/**
	 * Append a MenuItem to the ContextMenu
	 * @param newMenuItem the MenuItem to add to the ContextMenu
	 */
	void add(MenuItem newMenuItem);
}
