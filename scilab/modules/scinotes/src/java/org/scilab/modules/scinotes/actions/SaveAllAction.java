/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
