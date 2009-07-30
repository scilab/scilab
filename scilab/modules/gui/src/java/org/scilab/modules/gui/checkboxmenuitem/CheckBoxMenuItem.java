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

import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Interface for ChecBoxMenuItem associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface CheckBoxMenuItem extends MenuItem {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleCheckBoxMenuItem getAsSimpleCheckBoxMenuItem();

}
