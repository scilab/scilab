/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.pushbutton.PushButton;

/**
 * Interface for SimpleToolBar the associated object to Scilab GUIs ToolBar
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public interface SimpleToolBar {

    /**
     * Add a PushButton to the ToolBar.
     * @param pushButton the PushButton to add.
     */
    void add(PushButton pushButton);

    /**
     * Add a Separator to a toolbar
     */
    void addSeparator();

    /**
     * Set the element id for this toolbar
     * @param id the id of the corresponding toolbar object
     */
    void setElementId(int id);

    /**
     * Get the element id for this toolbar
     * @return id the id of the corresponding toolbar object
     */
    int getElementId();

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
