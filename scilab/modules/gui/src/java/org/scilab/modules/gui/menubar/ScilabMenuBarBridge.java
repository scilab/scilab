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
