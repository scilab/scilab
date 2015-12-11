/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * ExitAction class
 * @author Bruno JOFRET
 *
 */
public final class ExitAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -6434487252794798547L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ExitAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        doExit(getEditor());
    }

    /**
     * doExit
     * @param editor SciNotes
     */
    public static void doExit(SciNotes editor) {
        SciNotes.closeEditor(editor);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new ExitAction(label, editor), key);
    }
}
