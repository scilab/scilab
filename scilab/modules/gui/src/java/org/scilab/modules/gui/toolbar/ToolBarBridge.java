
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButtonBridge;

/**
 * Interface for ToolBarBridge the associated object to Scilab GUIs ToolBar
 * @author Marouane BEN JELLOUL
 */
public interface ToolBarBridge {
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add.
	 */
	void add(PushButtonBridge pushButton);
}
