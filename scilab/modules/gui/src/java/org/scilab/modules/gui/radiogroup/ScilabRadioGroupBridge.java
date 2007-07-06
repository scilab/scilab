
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.radiogroup;

import org.scilab.modules.gui.bridge.radiogroup.SwingScilabRadioGroup;

/**
 * Bridge for Scilab RadioGroup in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabRadioGroupBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabRadioGroupBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab RadioGroup
	 * @return the created RadioGroup
	 */
	public static RadioGroup createRadioGroup() {
		return new SwingScilabRadioGroup();
	}
}
