
/* Copyright INRIA 2007 */

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

}
