/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
        ScilabEditorPane sep = getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();
        int pos = sep.getCaretPosition();
        CommentManager com = sep.getCommentManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();

        doc.mergeEditsBegin();
        if (start == end) {
            com.commentLines(start, start);
        } else {
            int[] ret = com.commentLines(start, end);
            if (ret != null) {
                if (pos == start) {
                    sep.select(ret[1], ret[0]);
                } else {
                    sep.select(ret[0], ret[1]);
                }
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
