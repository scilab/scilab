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

package org.scilab.modules.gui.menuitem;

import org.scilab.modules.gui.menu.Menu;

/**
 * Interface for MenuItem associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface MenuItem extends Menu {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenuItem getAsSimpleMenuItem();
	
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the Mnemonic of the MenuItem
	 */
	void setMnemonic(char mnemonic);

}
