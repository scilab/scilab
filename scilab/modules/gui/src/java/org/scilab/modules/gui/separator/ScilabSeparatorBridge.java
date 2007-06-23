
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.separator;

import org.scilab.modules.gui.bridge.separator.SwingScilabSeparator;

/**
* Bridge for Scilab Separators in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabSeparatorBridge {

	/**
	 * Constructor
	 */
	protected ScilabSeparatorBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab Separator
	 * @return the created Separator
	 */
	public static Separator createSeparator() {
		return new SwingScilabSeparator();
	}

}
