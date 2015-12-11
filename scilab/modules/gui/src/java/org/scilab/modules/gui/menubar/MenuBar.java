/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for MenuBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface MenuBar extends UIElement {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleMenuBar getAsSimpleMenuBar();

    /**
     * Add a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     */
    void add(Menu newMenu);
}
