
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Scilab toolbar configuration
 * All methods needed to create a toolbar
 * @author Vincent COUVERT
 */
public interface ToolBarConfiguration {

	/**
	 * Add buttons to a toolbar
	 * @param tb the toolbar which the buttons will be added to
	 */
	void addPushButtons(ToolBar tb);
}
