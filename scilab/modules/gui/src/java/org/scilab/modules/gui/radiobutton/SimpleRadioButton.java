
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.radiobutton;

import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for RadioButtonBridge the associated object to Scilab GUIs RadioButton
 * @author Vincent COUVERT
 */
public interface SimpleRadioButton extends Widget {
	/**
	 * Set if the RadioButton is checked or not
	 * @param status true to set the RadioButton checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get the status of the RadioButton
	 * @return true if the RadioButton is checked
	 */
	boolean isChecked();
}
