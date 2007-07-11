
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.separator;

import org.scilab.modules.gui.bridge.ScilabBridge;

/**
* Class for Scilab Separators in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabSeparator implements Separator {

	/**
	 * Constructor
	 */
	protected ScilabSeparator() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Separator object
	 * @return the created Separator
	 */
	public static Separator createSeparator() {
		return ScilabBridge.createSeparator();
	}
}
