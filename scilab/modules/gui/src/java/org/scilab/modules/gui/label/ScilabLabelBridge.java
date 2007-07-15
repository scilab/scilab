
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.label;

import org.scilab.modules.gui.bridge.label.SwingScilabLabel;

/**
* Bridge for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabLabelBridge {

	/**
	 * Constructor
	 */
	protected ScilabLabelBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab Label
	 * @return the created Label
	 */
	public static Label createLabel() {
		return new SwingScilabLabel();
	}

}
