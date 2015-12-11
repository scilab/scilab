/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
