/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.listbox;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for ListBox associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface ListBox extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleListBox getAsSimpleListBox();

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

	/**
	 * Adjusts the view so that the element given by index is displayed at the top of the ListBox.
	 * @param index the index of the element to be displayed at the top of the ListBox.
	 */
	void setListBoxTop(int index);
	
	/**
	 * Gets the index of the element displayed at the top of the ListBox
	 * @return the index of the element displayed at the top of the ListBox
	 */
	int getListBoxTop();
}
