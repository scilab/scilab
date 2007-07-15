
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.layout;

import org.scilab.modules.gui.bridge.layout.SwingScilabFlowLayout;

/**
 * Bridge for Scilab FlowLayouts in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabFlowLayoutBridge {

	/**
	 * Constructor
	 */
	protected ScilabFlowLayoutBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab FlowLayout
	 * @return the created FlowLayout
	 */
	public static FlowLayout createFlowLayout() {
		return new SwingScilabFlowLayout();
	}
}
