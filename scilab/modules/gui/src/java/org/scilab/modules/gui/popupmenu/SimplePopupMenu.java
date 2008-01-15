
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBoxBridge the associated object to Scilab GUIs Checkbox
 * @author Vincent COUVERT
 */
public interface SimplePopupMenu extends Widget {
	/**
	 * Set the selected index of the PopupMenu
	 * @param index the index of the item to be selected
	 */
	void setSelectedIndex(int index);
	
	/**
	 * Get the selected index of the PopupMenu
	 * @return the index of the item selected
	 */
	int getSelectedIndex();
	
	/**
	 * Get the text of all the PopupMenu items
	 * @return the text items
	 */
	String[] getAllItemsText();

	/**
	 * Get the number of items in the PopupMenu
	 * @return the number of items
	 */
	int getNumberOfItems();

	/**
	 * Set the text of the PopupMenu items
	 * @param text the text of the items
	 */
	void setText(String[] text);
}
