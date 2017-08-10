/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2010 - Allan Simon
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

package org.scilab.modules.ui_data.variablebrowser.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.ui_data.actions.DefaultAction;
import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser;

@SuppressWarnings(value = { "serial" })
public class CloseAction extends DefaultAction {

    /** Name of the action */
    public static final String NAME = UiDataMessages.CLOSE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_W;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();


    /**
     * Menu for diagram menubar
     * @param scilabGraph associated diagram
     * @return the menu
     */
    public static MenuItem createMenu() {
        return createMenu(new CloseAction());
    }


    @Override
    public void actionPerformed(ActionEvent e) {
        ScilabVariableBrowser.getVariableBrowser().close();
    }
}
