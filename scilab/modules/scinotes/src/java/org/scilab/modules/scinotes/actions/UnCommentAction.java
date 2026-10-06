/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
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
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.CommentManager;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * UnCommentAction Class
 * @author Calixte DENIZET
 *
 */
public final class UnCommentAction extends DefaultAction {
    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public UnCommentAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        if (sep instanceof MonacoEditorPane) {
            ((MonacoEditorPane) sep).uncommentSelection();
            return;
        }

        int start = sep.getSelectionStart();
        int end  = sep.getSelectionEnd();
        int pos = sep.getCaretPosition();
        CommentManager com = sep.getCommentManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        int[] ret;

        doc.mergeEditsBegin();
        if (start == end) {
            ret = com.uncommentLines(start, start);
            sep.setCaretPosition(ret[0]);
        } else {
            try {
             end   -= doc.getText(end-1,1).charAt(0)=='\n' ? 1 : 0;
            } catch (Exception e) {} // 0<=start<end hence end>0 exception won't occur
            ret = com.uncommentLines(start, end);
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
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new UnCommentAction(label, editor), key);
    }
}
