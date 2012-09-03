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
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * ToLowerCaseAction Class
 * @author Calixte DENIZET
 *
 */
@SuppressWarnings(value = { "serial" })
public final class ToLowerCaseAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ToLowerCaseAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        String str = sep.getSelectedText();
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();

        if (str != null) {
            doc.mergeEditsBegin();
            sep.replaceSelection(str.toLowerCase());
            sep.select(start, end);
            doc.mergeEditsEnd();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new ToLowerCaseAction(label, editor), key);
    }
}
