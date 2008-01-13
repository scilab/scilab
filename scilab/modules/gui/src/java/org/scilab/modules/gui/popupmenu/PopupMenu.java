
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for PopupMenu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface PopupMenu extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimplePopupMenu getAsSimplePopupMenu();
	
	/**
	 * Set the selected index of the ListBox
	 * @param index the index of the item to be selected
	 */
	void setSelectedIndex(int index);
	
	/**
	 * Get the selected index of the ListBox
	 * @return the index of the item selected
	 */
	int getSelectedIndex();
}
