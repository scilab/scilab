/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

@SuppressWarnings(value = { "serial" })
public class TListFilteringAction extends FilteringAction {
    /** Name of the action */
    public static final String NAME = UiDataMessages.TLIST;


    /**
     * Constructor
     *
     */
    public TListFilteringAction() {
        super();
    }
    /**
     * Create checkbox menu for the graph menu bar
     *
     * @return the menu
     */
    public static CheckBoxMenuItem createCheckBoxMenu() {
        CheckBoxMenuItem menu = createCheckBoxMenu( new TListFilteringAction());
        return menu;
    }

}