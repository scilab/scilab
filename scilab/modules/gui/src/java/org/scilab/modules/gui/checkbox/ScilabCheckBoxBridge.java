
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.checkbox;

import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;

/**
 * Bridge for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabCheckBoxBridge {

	/**
	 * Constructor
	 */
	protected ScilabCheckBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab CheckBox
	 * @return the created CheckBox
	 */
	public static CheckBox createCheckBox() {
		return new SwingScilabCheckBox();
	}
}
