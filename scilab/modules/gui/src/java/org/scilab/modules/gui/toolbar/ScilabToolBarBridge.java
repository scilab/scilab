
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;

/**
 * Bridge for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabToolBarBridge {

	/**
	 * Constructor
	 */
	protected ScilabToolBarBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab ToolBar
	 * @return the created ToolBar
	 */
	public static ToolBar createToolBar() {
		return new SwingScilabToolBar();
	}
}
