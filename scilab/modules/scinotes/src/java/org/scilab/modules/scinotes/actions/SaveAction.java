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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.pushbutton.PushButton;
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
     * @param editor SciNotes
     */
    private SaveAction(SciNotes editor) {
        super(SciNotesMessages.SAVE, editor);
    }

    /**
     * Create Menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.SAVE, null, new SaveAction(editor), key);
    }

    /**
     * Create Button
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(SciNotes editor) {
        return createButton(SciNotesMessages.SAVE, "media-floppy.png", new SaveAction(editor));
    }

    /**
     * DoAction
     */
    public void doAction() {
        if (!getEditor().save(getEditor().getTabPane().getSelectedIndex(), true)) {
            ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_SAVE_FILE,
                                   SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }
}
