
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.textbox;

import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;

/**
 * Bridge for Scilab TextBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabTextBoxBridge {

	/**
	 * Constructor
	 */
	protected ScilabTextBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab TextBox
	 * @return the created TextBox
	 */
	public static TextBox createTextBox() {
		return new SwingScilabTextBox();
	}
}
