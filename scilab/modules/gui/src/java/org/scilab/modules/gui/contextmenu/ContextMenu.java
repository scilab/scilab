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

package org.scilab.modules.gui.contextmenu;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.widget.Widget;

/**
 * Interface for ContextMenu associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 */
public interface ContextMenu extends Widget {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleContextMenu getAsSimpleContextMenu();

    /**
     * Append a MenuItem to the ContextMenu
     * @param newMenuItem the MenuItem to add to the ContextMenu
     */
    void add(MenuItem newMenuItem);

    /**
     * Append a Menu to the ContextMenu
     * @param newMenu the Menu to add to the ContextMenu
     */
    void add(Menu newMenu);
}
