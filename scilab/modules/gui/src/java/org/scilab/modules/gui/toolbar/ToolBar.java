/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for ToolBar associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface ToolBar extends UIElement {
    // TODO : Add the methods that are usefull for a ToolBar

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleToolBar getAsSimpleToolBar();

    /**
     * Add a PushButton to the ToolBar.
     * @param pushButton the PushButton to add .
     */
    //void add(SwingScilabPushButton pushButton);

    /**
     * Add a Separator to a toolbar
     */
    void addSeparator();
}
