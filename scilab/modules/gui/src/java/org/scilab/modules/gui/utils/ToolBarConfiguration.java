/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Scilab toolbar configuration
 * All methods needed to create a toolbar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
