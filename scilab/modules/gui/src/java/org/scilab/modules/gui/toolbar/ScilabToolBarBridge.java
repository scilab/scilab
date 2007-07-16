
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.pushbutton.SimplePushButton;

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
	public static SimpleToolBar createToolBar() {
		return new SwingScilabToolBar();
	}
	
	/**
	 * Append a pushButton to a toolBar
	 * @param toolBar the ToolBar which we want to add the pushButton to
	 * @param pushButton the PushButton to add to the toolBar
	 */
	public static void add(SimpleToolBar toolBar, SimplePushButton pushButton) {
		toolBar.add(pushButton);
	}
}
