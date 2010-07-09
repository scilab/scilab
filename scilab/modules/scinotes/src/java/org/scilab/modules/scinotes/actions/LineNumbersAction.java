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
package org.scilab.modules.scinotes.actions;

import java.util.StringTokenizer;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

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

    private static String labelWhereami = "";
    private static String labelNormal = "";
    private static String labelNoLine = "";

    private MenuItem menu;
    private int state = 1;

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public LineNumbersAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().setWhereamiLineNumbering(state);
        ConfigSciNotesManager.saveLineNumberingState(state);
        setMenu();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        StringTokenizer tokens = new StringTokenizer(label, ";");
        labelWhereami = tokens.nextToken();
        labelNormal = tokens.nextToken();
        labelNoLine = tokens.nextToken();
        LineNumbersAction ln = new LineNumbersAction(labelWhereami, editor);
        MenuItem mi = createMenu(label, null, ln, key);
        ln.state = ConfigSciNotesManager.getLineNumberingState();
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
            menu.setText(labelNormal);
            break;
        case 1 :
            menu.setText(labelWhereami);
            break;
        default :
            menu.setText(labelNoLine);
        }
    }
}
