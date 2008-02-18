/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for ToolBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface ToolBar extends UIElement {
	// TODO : Add the methods that are usefull for a ToolBar
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleToolBar getAsSimpleToolBar();
	
	/**
	 * Add a PushButton to the ToolBar.
	 * @param pushButton the PushButton to add .
	 */
	void add(PushButton pushButton);

	/**
	 * Add a Separator to a toolbar
	 */
	void addSeparator();
}
