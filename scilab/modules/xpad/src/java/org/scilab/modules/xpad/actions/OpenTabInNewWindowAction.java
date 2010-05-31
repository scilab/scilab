/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Display about box for the text editor
 * @author Calixte DENIZET
 */
public final class OpenTabInNewWindowAction extends DefaultAction {

    /**
     * Constructor
     * @param editor associated editor
     */
    public OpenTabInNewWindowAction(Xpad editor) {
        super(XpadMessages.OPEN_TAB_IN_NEW_WINDOW, editor);
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.OPEN_TAB_IN_NEW_WINDOW, null, new OpenTabInNewWindowAction(editor), key);
    }

    /**
     * Action
     */
    public void doAction() {
        getEditor().cloneAndCloseCurrentTab(false);
    }
}
