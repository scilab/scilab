/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
 * LineNumbersAction Class
 * @author Bruno JOFRET
 *
 */
public final class LineNumbersAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -2778300710964013775L;

    private MenuItem menu;
    private boolean state;

    /**
     * Construtor
     * @param editor Xpad
     */
    private LineNumbersAction(Xpad editor) {
        super(XpadMessages.LINE_NUMBERS_WHEREAMI, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        state = !state;
        getEditor().setWhereamiLineNumbering(state);
        if (state) {
            menu.setText(XpadMessages.LINE_NUMBERS_NOWHEREAMI);
        } else {
            menu.setText(XpadMessages.LINE_NUMBERS_WHEREAMI);
        }
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        LineNumbersAction ln = new LineNumbersAction(editor);
        MenuItem mi = createMenu(XpadMessages.LINE_NUMBERS_WHEREAMI, null, ln, key);
        ln.menu = mi;
        return mi;
    }
}
