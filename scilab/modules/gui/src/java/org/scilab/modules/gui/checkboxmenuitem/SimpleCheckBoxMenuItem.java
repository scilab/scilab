/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.checkboxmenuitem;

import org.scilab.modules.gui.menuitem.SimpleMenuItem;

/**
 * Interface for SimpleCheckBoxMenuItem the associated object to Scilab GUIs CheckBoxMenuItem
 * @author Vincent COUVERT
 */
public interface SimpleCheckBoxMenuItem extends SimpleMenuItem {

    /**
     * Set if the menu item is checked or not
     * @param status true if the menu item is checked
     */
    void setChecked(boolean status);

    /**
     * Get if the menu item is checked or not
     * @return true if the menu item is checked
     */
    boolean isChecked();

}
