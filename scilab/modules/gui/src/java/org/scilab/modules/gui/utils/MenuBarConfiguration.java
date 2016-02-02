/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
