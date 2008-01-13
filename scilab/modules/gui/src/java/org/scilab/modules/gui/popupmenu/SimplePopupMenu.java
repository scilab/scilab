
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.popupmenu;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBoxBridge the associated object to Scilab GUIs Checkbox
 * @author Vincent COUVERT
 */
public interface SimplePopupMenu extends Widget {
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
