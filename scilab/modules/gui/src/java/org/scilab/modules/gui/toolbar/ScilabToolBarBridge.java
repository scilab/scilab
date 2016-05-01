/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;

/**
 * Bridge for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public class ScilabToolBarBridge {

    /**
     * Constructor
     */
    protected ScilabToolBarBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a Scilab ToolBar
     * @return the created ToolBar
     */
    public static SimpleToolBar createToolBar() {
        return new SwingScilabToolBar();
    }

    /**
     * Add a Separator to a Toolbar
     * @param toolbar the toolbar which we want to add the Separator to
     */
    public static void addSeparator(ToolBar toolbar) {
        toolbar.getAsSimpleToolBar().addSeparator();
    }

    /**
     * Set the visibility of the Toolbar
     * @param toolbar the ToolBar
     * @param newVisibleState true to set the toolbar visible
     */
    public static void setVisible(ToolBar toolbar, boolean newVisibleState) {
        toolbar.getAsSimpleToolBar().setVisible(newVisibleState);
    }

    /**
     * Get the visibility of the TooBar
     * @param toolbar the ToolBar
     * @return true if the ToolBar is Visible
     */
    public static boolean isVisible(ToolBar toolbar) {
        return toolbar.getAsSimpleToolBar().isVisible();
    }



}
