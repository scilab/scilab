
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.checkbox;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for CheckBoxBridge the associated object to Scilab GUIs Checkbox
 * @author Vincent COUVERT
 */
public interface SimpleCheckBox extends Widget {
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
