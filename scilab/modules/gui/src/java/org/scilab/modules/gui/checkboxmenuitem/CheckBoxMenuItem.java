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

import org.scilab.modules.gui.menuitem.MenuItem;

/**
 * Interface for ChecBoxMenuItem associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface CheckBoxMenuItem extends MenuItem {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleCheckBoxMenuItem getAsSimpleCheckBoxMenuItem();

}
