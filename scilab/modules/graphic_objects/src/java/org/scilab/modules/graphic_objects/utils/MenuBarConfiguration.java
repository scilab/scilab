/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.graphic_objects.utils;

/**
 * Scilab menubar configuration
 * All methods needed to create a menubar
 * @author Vincent COUVERT
 */
public interface MenuBarConfiguration {

    /**
     * Add menus to a menubar
     * @param parentId the parent figure UID
     */
    void addMenus(Integer parentId);

    /**
     * Read submenus data in the XML file
     * @param parentId the parent figure UID
     * @param index the index of the parent in menu list
     */
    void addSubMenus(Integer parentId, int index);
}
