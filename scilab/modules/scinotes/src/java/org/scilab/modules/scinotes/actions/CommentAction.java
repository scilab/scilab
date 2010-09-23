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
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.CommentManager;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * CommentAction Class
 * @author Bruno JOFRET
 *
 */
public final class CommentAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -7258307088402814986L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CommentAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end   = sep.getSelectionEnd();
        CommentManager com = sep.getCommentManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();

        doc.mergeEditsBegin();
        if (start == end) {
            com.commentText(start);
        } else {
            int[] ret = com.commentLines(start, end - 1);
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
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CommentAction(label, editor), key);
    }
}
