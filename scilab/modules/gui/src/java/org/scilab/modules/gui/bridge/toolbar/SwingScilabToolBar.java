
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.toolbar;

import javax.swing.JToolBar;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.pushbutton.SimplePushButton;
import org.scilab.modules.gui.toolbar.SimpleToolBar;

/**
 * Swing implementation for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabToolBar extends JToolBar implements SimpleToolBar {

	/**
	 * Constructor
	 */
	public SwingScilabToolBar() {
		super();
		setFloatable(false);
	}
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	public void add(SimplePushButton pushButton) {
		super.add((SwingScilabPushButton) pushButton);
	}
	
}
