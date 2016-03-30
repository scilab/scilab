/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Scilab toolbar configuration
 * All methods needed to create a toolbar
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

package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Scilab toolbar configuration
 * All methods needed to create a toolbar
 * @author Vincent COUVERT
 */
public interface ToolBarConfiguration {

    /**
     * Add buttons to a toolbar
     * @param tb the toolbar which the buttons will be added to
     */
    void addPushButtons(ToolBar tb);
}
