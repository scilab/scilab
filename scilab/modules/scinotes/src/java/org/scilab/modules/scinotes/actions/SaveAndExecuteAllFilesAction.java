/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;

/**
 * SaveAndExecuteAllFilesAction class
 * @author Calixte DENIZET
 *
 */
@SuppressWarnings(value = { "serial" })
public class SaveAndExecuteAllFilesAction extends SaveAndExecuteFileAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SaveAndExecuteAllFilesAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes editor = getEditor();
        int n = editor.getTabPane().getTabCount();
        boolean saved = true;
        for (int i = 0; i < n; i++) {
            ScilabEditorPane pane = editor.getTextPane(i);
            if (((ScilabDocument) pane.getDocument()).isContentModified()) {
                editor.getTabPane().setSelectedIndex(i);
                saved = saved && editor.save(i, true);
            }
        }
        if (saved) {
            for (int i = 0; i < n; i++) {
                executeFile(editor, editor.getTextPane(i).getName());
            }
        }
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new SaveAndExecuteAllFilesAction(tooltip, editor));
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SaveAndExecuteAllFilesAction(label, editor), key);
    }
}
