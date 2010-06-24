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
 *
 * @author Bruno JOFRET
 *
 */
public final class SaveAsAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 8327808176820789884L;

    /**
     * Constructor
     * @param editor SciNotes
     */
    private SaveAsAction(SciNotes editor) {
        super(SciNotesMessages.SAVE_AS, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        if (!getEditor().saveAs(getEditor().getTextPane())) {
            ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_SAVE_FILE,
                                   SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }

    /**
     * CreateMenu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.SAVE_AS, null, new SaveAsAction(editor), key);
    }

    /**
     * Create Button
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(SciNotes editor) {
        return createButton(SciNotesMessages.SAVE_AS, "document-save-as.png", new SaveAsAction(editor));
    }
}
