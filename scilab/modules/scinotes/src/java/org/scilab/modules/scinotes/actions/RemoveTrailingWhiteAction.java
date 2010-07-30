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
import org.scilab.modules.scinotes.TrailingWhiteManager;

/**
 * RemoveTrailingWhiteAction Class
 * @author Calixte DENIZET
 */
public final class RemoveTrailingWhiteAction extends DefaultAction {

    /**
     * Construtor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public RemoveTrailingWhiteAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();
        TrailingWhiteManager trailing = sep.getTrailingWhiteManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();

        doc.mergeEditsBegin();
        if (start == end) {
            int ret = trailing.removeTrailingWhite(start);
            if (ret != -1) {
                sep.setCaretPosition(ret);
            }
        } else {
            int[] ret = new int[2];
            ret = trailing.removeTrailingWhite(start, end - 1);
            if (ret != null) {
                sep.setSelectionStart(ret[0]);
                sep.setSelectionEnd(ret[1]);
            }
        }
        doc.mergeEditsEnd();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new RemoveTrailingWhiteAction(label, editor), key);
    }
}
