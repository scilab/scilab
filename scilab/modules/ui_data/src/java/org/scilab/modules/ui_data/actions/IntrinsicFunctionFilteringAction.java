/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.ui_data.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * Manage INTRINSIC FUNCTIONS display
 * @author Vincent COUVERT
 *
 */
public class IntrinsicFunctionFilteringAction extends FilteringAction {

    /** Name of the action */
    public static final String NAME = UiDataMessages.INTRINSIC_FUNCTION;

    private static final long serialVersionUID = -7371889324166887650L;

    /**
     * Constructor
     */
    public IntrinsicFunctionFilteringAction() {
        super();
    }

    /**
     * Create checkbox menu for the graph menu bar
     * @return the menu
     */
    public static CheckBoxMenuItem createCheckBoxMenu() {
        CheckBoxMenuItem menu = createCheckBoxMenu(new IntrinsicFunctionFilteringAction());
        return menu;
    }

}
