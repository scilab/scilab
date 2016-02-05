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

package org.scilab.modules.gui.bridge.toolbar;

import javax.swing.JToolBar;

import org.scilab.modules.gui.toolbar.SimpleToolBar;

/**
 * Swing implementation for Scilab ToolBar in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabToolBar extends JToolBar implements SimpleToolBar {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public SwingScilabToolBar() {
        super();
        setFloatable(false);
        setRollover(true); /* Bug 2752 */
    }

    /**
     * Add a Separator to a Toolbar
     * @see org.scilab.modules.gui.toolbar.ToolBar#addSeparator(org.scilab.modules.gui..)
     */
    public void addSeparator() {
        super.addSeparator();
    }
}
