/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * CloseAllAction Class
 * @author Allan CORNET
 *
 */
public final class CloseAllAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 7134703185408271944L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CloseAllAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes ed = getEditor();
        while (ed.getTabPane().getTabCount() != 0) {
            if (!ed.closeTabAt(ed.getTabPane().getSelectedIndex())) {
                return;
            }
        }

        // Close the last opened file create a new file named "Untitled 1"
        if (ed.getTabPane().getTabCount() == 0) {
            ed.addEmptyTab();
            ConfigSciNotesManager.saveToOpenFiles(ed.getTextPane().getName(), ed, ed.getTextPane());
        }
    }

    /**
     * Create menu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CloseAllAction(label, editor), key);
    }
}
