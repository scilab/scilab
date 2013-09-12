/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.menu.Menu;

/**
 * Bridge for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBarBridge {

    /**
     * Constructor
     */
    protected ScilabMenuBarBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Creates a Scilab MenuBar
     * @return the created MenuBar
     */
    public static SimpleMenuBar createMenuBar() {
        return new SwingScilabMenuBar();
    }

    /**
     * Append a Menu to a MenuBar
     * @param menuBar the MenuBar which we want to add the newMenu to
     * @param newMenu the Menu to add to the MenuBar
     */
    public static void add(MenuBar menuBar, Menu newMenu) {
        menuBar.getAsSimpleMenuBar().add(newMenu);
    }

}
