
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.layout.SwingScilabBorderLayout;

/**
 * Bridge for Scilab BorderLayout in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabBorderLayoutBridge {

	/**
	 * Constructor
	 */
	protected ScilabBorderLayoutBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab BorderLayout
	 * @return the created BorderLayout
	 */
	public static BorderLayout createBorderLayout() {
		return new SwingScilabBorderLayout();
	}
}
