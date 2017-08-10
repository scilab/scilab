/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;

/**
 * Interface for SimpleMenuBar the associated object to Scilab GUIs MenuBar
 * @author Marouane BEN JELLOUL
 */
public interface SimpleMenuBar {

    /**
     * Add a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     */
    void add(Menu newMenu);

    /**
     * Enable/Disable a menu giving its name
     * @param menuName the name of the menu
     * @param status true to enable the menu
     */
    void setMenuEnabled(String menuName, boolean status);

    /**
     * Disable a MenuItem of a Scilab root window giving its parent name and position
     * @param parentMenuName the name of the parent menu
     * @param menuItemPosition the name of the parent menu
     * @param status true to set the menu enabled
     */
    void setSubMenuEnabled(String parentMenuName, int menuItemPosition, boolean status);

    /**
     * Remove a menu giving its name
     * @param menuName the name of the menu
     */
    void removeMenu(String menuName);

    /**
     * Set the visibility of the Toolbar
     * @param newVisibleState true to set the toolbar visible
     */
    void setVisible(boolean newVisibleState);
}
