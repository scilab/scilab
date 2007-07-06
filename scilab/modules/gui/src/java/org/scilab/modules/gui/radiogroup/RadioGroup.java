
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.radiogroup;

import org.scilab.modules.gui.radiobutton.RadioButton;

/**
 * Interface for RadioGroup associated to objects in Scilab GUIs
 * @author Marouane BEN JELLOUL
 */
public interface RadioGroup {

	/**
	 * add a RadioButton in a RadioGroup.
	 * @param button the button to add
	 */
	void add(RadioButton button);
	
	/**
	 * remove a RadioButton from a RadioGroup.
	 * @param button the button to remove
	 */
	void remove(RadioButton button);
}
