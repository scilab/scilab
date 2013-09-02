/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;

/**
 * Scilab menubar configuration
 * All methods needed to create a menubar
 * @author Vincent COUVERT
 */
public interface MenuBarConfiguration {

    /**
     * Add menus to a menubar
     * @param mb the menubar which the menus will be added to
     */
    void addMenus(MenuBar mb);

    /**
     * Read submenus data in the XML file
     * @param menu the parent menu for submenus
     * @param index the index of the parent in menu list
     */
    void addSubMenus(Menu menu, int index);
}
