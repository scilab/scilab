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

package org.scilab.modules.gui.menu;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for Menu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Menu extends Widget {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMenu getAsSimpleMenu();
	
	/**
	 * Append a MenuItem to the Menu
	 * @param newMenuItem the MenuItem to add to the Menu
	 */
	void add(MenuItem newMenuItem);
	
	/**
	 * Append a CheckBoxMenuItem to the Menu
	 * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
	 */
	void add(CheckBoxMenuItem newCheckBoxMenuItem);
	
	/**
	 * Append a subMenu to the Menu
	 * @param newSubMenu the subMenu to append to the Menu
	 */
	void add(Menu newSubMenu);
	
	/**
	 * set a mnemonic to the Menu
	 * @param mnemonic the new mnemonic of the Menu
	 */
	void setMnemonic(int mnemonic);
	
	/**
	 * Add a Separator to a Menu
	 */
	void addSeparator();
	
	/**
	 * Set if the Menu is checked or not
	 * @param status true if the Menu is checked
	 */
	void setChecked(boolean status);
	
	/**
	 * Get if the Menu is checked or not
	 * @return true if the Menu is checked
	 */
	boolean isChecked();
	
	/**
	 * Retrieve the CallBack associated to this MenuItem
	 * @return the CallBack
	 */
	CallBack getCallback();
}
