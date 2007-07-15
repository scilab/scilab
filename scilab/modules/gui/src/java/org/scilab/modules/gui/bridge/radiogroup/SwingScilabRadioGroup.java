
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.radiogroup;

import javax.swing.ButtonGroup;
import javax.swing.JRadioButton;

import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.radiogroup.RadioGroup;

/**
 * Swing implementation for Scilab RadioGroup in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabRadioGroup extends ButtonGroup implements RadioGroup {
	
	/**
	 * Constructor
	 */
	public SwingScilabRadioGroup() {
		super();
	}
	
	/**
	 * Add a RadioButton to a RadioGroup
	 * @param button the button to add
	 */
	public void add(RadioButton button) {
		super.add((SwingScilabRadioButton) button);
	}
	
	/**
	 * Remove a RadioButton from a RadioGroup.
	 * @param button the button to remove
	 */
	public void remove(RadioButton button) {
		super.remove((SwingScilabRadioButton) button);
	}
	
}
