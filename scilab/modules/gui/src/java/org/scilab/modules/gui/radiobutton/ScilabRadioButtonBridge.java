
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.radiobutton;

import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;

/**
 * Bridge for Scilab RadioButton in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabRadioButtonBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabRadioButtonBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab RadioButton
	 * @return the created RadioButton
	 */
	public static RadioButton createRadioButton() {
		return new SwingScilabRadioButton();
	}
}
