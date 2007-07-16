
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Class for Scilab ToolBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabToolBar implements ToolBar {

	private SimpleToolBar component;
	
	/**
	 * Constructor
	 */
	protected ScilabToolBar() {
		component = ScilabBridge.createToolBar();
	}

	/**
	 * Creates a Scilab MenuBar object
	 * @return the created MenuBar
	 */
	public static ToolBar createToolBar() {
		return new ScilabToolBar();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleToolBar getAsSimpleToolBar() {
		return component;
	}
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	public void add(PushButton pushButton) {
		ScilabBridge.add(component, pushButton.getPushButtonBridge());
	}
}
