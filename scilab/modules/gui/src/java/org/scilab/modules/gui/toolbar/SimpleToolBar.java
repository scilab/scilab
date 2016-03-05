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

/**
 * Interface for SimpleToolBar the associated object to Scilab GUIs ToolBar
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public interface SimpleToolBar {

    /**
     * Add a Separator to a toolbar
     */
    void addSeparator();

    /**
     * Get the visibility of the TooBar
     * @return true if the ToolBar is Visible
     */
    boolean isVisible();

    /**
     * Set the visibility of the Toolbar
     * @param newVisibleState true to set the toolbar visible
     */
    void setVisible(boolean newVisibleState);

}
