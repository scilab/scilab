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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * ReloadAction class
 * @author Calixte DENIZET
 *
 */
public final class ReloadAction extends DefaultAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private ReloadAction(SciNotes editor) {
        super(SciNotesMessages.RELOAD, editor);
    }

    /**
     * Create Menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.RELOAD, null, new ReloadAction(editor), key);
    }

    /**
     * DoAction
     */
    public void doAction() {
        getEditor().reload(getEditor().getTabPane().getSelectedIndex());
        getEditor().getTextPane().updateInfosWhenFocused();
    }
}
