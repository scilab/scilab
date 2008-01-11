
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

}
