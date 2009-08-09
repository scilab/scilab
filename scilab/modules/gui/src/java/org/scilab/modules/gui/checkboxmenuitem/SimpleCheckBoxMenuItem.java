/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.checkboxmenuitem;

import org.scilab.modules.gui.menuitem.SimpleMenuItem;

/**
 * Interface for SimpleCheckBoxMenuItem the associated object to Scilab GUIs CheckBoxMenuItem
 * @author Vincent COUVERT
 */
public interface SimpleCheckBoxMenuItem extends SimpleMenuItem {
	
	/**
	 * Set if the menu item is checked or not
	 * @param status true if the menu item is checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get if the menu item is checked or not
	 * @return true if the menu item is checked
	 */
	boolean isChecked();
	
}
