/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * LineNumbersAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class LineNumbersAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -2778300710964013775L;

    private MenuItem menu;
    private int state = 1;

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
        getEditor().setWhereamiLineNumbering(state);
        ConfigXpadManager.saveLineNumberingState(state);
        setMenu();
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
        ln.state = ConfigXpadManager.getLineNumberingState();
        ln.menu = mi;
        ln.setMenu();
        return mi;
    }

    /**
     * Set the menu
     */
    private void setMenu() {
        state = (state + 1) % 3;
        switch (state) {
        case 0 :
            menu.setText(XpadMessages.LINE_NUMBERS_NOWHEREAMI);
            break;
        case 1 :
            menu.setText(XpadMessages.LINE_NUMBERS_WHEREAMI);
            break;
        default :
            menu.setText(XpadMessages.NO_LINE_NUMBERS);
        }
    }
}
