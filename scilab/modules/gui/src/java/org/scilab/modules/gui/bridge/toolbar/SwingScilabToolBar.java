
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.toolbar;

import javax.swing.JToolBar;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Swing implementation for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabToolBar extends JToolBar implements ToolBar {

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
	public void add(PushButton pushButton) {
		super.add((SwingScilabPushButton) pushButton);
	}
	
//	/**
//	 * Add a SwingScilabPushButton to the ToolBar.
//	 * @param pushButton the SwingScilabPushButton to add.
//	 */
//	public void add(SwingScilabPushButton pushButton) {
//		this.add((JButton) pushButton);
//	}
}
