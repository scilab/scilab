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

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for SimpleMenuItem the associated object to Scilab GUIs MenuItem
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuItem extends Widget {
	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic for  the MenuItem
	 */
	void setMnemonic(char mnemonic);

	/**
	 * set a mnemonic to a MenuItem
	 * @param mnemonic the mnemonic for  the MenuItem
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Set if the menu item is enabled or not
	 * @param status true if the menu item is enabled
	 */
	void setEnabled(boolean status);
	
	/**
	 * Add a MenuItem to this MenuItem
	 * @param childMenuItem the MenuItem we want to add
	 */
	void add(MenuItem childMenuItem);

	/**
	 * Add a Menu to this MenuItem
	 * @param childMenu the Menu we want to add
	 */
	void add(Menu childMenu);

	/**
	 * Destroy the MenuItem
	 */
	void destroy();

	/**
	 * Retrieve the CallBack associated to this MenuItem
	 * @return the CallBack
	 */
	CallBack getCallback();
}
