
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.pushbutton.PushButton;

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
	public static void add(ToolBar toolBar, PushButton pushButton) {
		toolBar.getAsSimpleToolBar().add(pushButton);
	}

	/**
	 * Add a Separator to a Toolbar
	 * @param toolbar the toolbar which we want to add the Separator to
	 */
	public static void addSeparator(ToolBar toolbar) {
		toolbar.getAsSimpleToolBar().addSeparator();
	}

}
