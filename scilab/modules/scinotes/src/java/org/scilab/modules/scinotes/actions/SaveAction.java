/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * SaveAction class
 * @author Bruno JOFRET
 *
 */
public final class SaveAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 1638973738114812027L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SaveAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Create Menu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SaveAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new SaveAction(tooltip, editor));
    }

    /**
     * DoAction
     */
    public void doAction() {
        if (!getEditor().save(getEditor().getTabPane().getSelectedIndex(), true)) {
            ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_SAVE_FILE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }
}
