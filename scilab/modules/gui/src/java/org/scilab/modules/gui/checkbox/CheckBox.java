
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.checkbox;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBox associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface CheckBox extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleCheckBox getAsSimpleCheckBox();
	
	/**
	 * Set if the CheckBox is checked or not
	 * @param status true to set the CheckBox checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get the status of the CheckBox
	 * @return true if the CheckBox is checked
	 */
	boolean isChecked();
}
