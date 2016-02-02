/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises
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

package org.scilab.modules.ui_data.newsfeed.actions;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

import org.scilab.modules.ui_data.utils.UiDataMessages;
import org.scilab.modules.ui_data.newsfeed.NewsFeedTab;

import org.scilab.modules.commons.gui.ScilabKeyStroke;

public class CloseAction extends CommonCallBack {

    private static final String LABEL = UiDataMessages.CLOSE;
    private static final String ICON = null;
    private static final char MNEMONIC = 'F';
    private static final String KEY = "OSSCKEY W";

    public CloseAction() {
        super("");
    }

    public static MenuItem createMenuItem() {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(LABEL);
        menuItem.setMnemonic(MNEMONIC);
        menuItem.setCallback(getCallBack());
        ((SwingScilabMenuItem) menuItem.getAsSimpleMenuItem()).setAccelerator(ScilabKeyStroke.getKeyStroke(KEY));
        return menuItem;
    }

    public static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = CloseAction.class.getConstructor().newInstance();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return callback;
    }

    public void callBack() {
        ClosingOperationsManager.startClosingOperation(NewsFeedTab.getInstance());
    }
}
