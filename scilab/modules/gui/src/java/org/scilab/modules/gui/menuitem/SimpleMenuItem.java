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

import org.scilab.modules.gui.menu.SimpleMenu;

/**
 * Interface for SimpleMenuItem the associated object to Scilab GUIs MenuItem
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuItem extends SimpleMenu {
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic for  the MenuItem
	 */
	void setMnemonic(char mnemonic);
}
