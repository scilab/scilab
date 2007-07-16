
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.toolbar;

import javax.swing.JToolBar;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.pushbutton.PushButtonBridge;
import org.scilab.modules.gui.toolbar.ToolBarBridge;

/**
 * Swing implementation for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabToolBar extends JToolBar implements ToolBarBridge {

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
	public void add(PushButtonBridge pushButton) {
		super.add((SwingScilabPushButton) pushButton);
	}
	
}
