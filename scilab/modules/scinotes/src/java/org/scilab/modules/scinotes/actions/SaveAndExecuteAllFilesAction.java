/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
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
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
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
