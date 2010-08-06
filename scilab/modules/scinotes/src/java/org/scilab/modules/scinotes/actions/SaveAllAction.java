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
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 *
 * @author Allan CORNET
 * SaveAllAction class
 */
public final class SaveAllAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 7134703185408281234L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SaveAllAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        int currentIndex = getEditor().getTabPane().getSelectedIndex();

        for (int i = 0; i < getEditor().getTabPane().getTabCount(); i++) {
            getEditor().getTabPane().setSelectedIndex(i);
            if (!getEditor().save(getEditor().getTabPane().getSelectedIndex(), true)) {
                ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_SAVE_FILE,
                                       SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
            }
        }
        getEditor().getTabPane().setSelectedIndex(currentIndex);

    }

    /**
     * CreateMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SaveAllAction(label, editor), key);
    }
}
