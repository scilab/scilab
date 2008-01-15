
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.listbox;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBoxBridge the associated object to Scilab GUIs Checkbox
 * @author Vincent COUVERT
 */
public interface SimpleListBox extends Widget {
	
	/**
	 * Set if more than one item can be selected in a ListBox
	 * @param status true if multiple selection is enabled
	 */
	void setMultipleSelectionEnabled(boolean status);

	/**
	 * Set the selected indices of the ListBox
	 * @param indices the indices of the items to be selected
	 */
	void setSelectedIndices(int[] indices);
	
	/**
	 * Get the selected indices of the ListBox
	 * @return the indices of the items selected
	 */
	int[] getSelectedIndices();

	/**
	 * Get the number of items selected in the ListBox
	 * @return the number of items selected
	 */
	int getSelectionSize();
	
	/**
	 * Get the text of all the list items
	 * @return the text items
	 */
	String[] getAllItemsText();

	/**
	 * Get the number of items in the list
	 * @return the number of items
	 */
	int getNumberOfItems();

	/**
	 * Set the text of the list items
	 * @param text the text of the items
	 */
	void setText(String[] text);
}
