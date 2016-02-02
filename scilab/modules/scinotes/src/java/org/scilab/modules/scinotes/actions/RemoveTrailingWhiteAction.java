/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

import java.io.IOException;

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
@SuppressWarnings(value = { "serial" })
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
        ScilabEditorPane sep = getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();
        int pos = sep.getCaretPosition();
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
     * @return an action to reindent a document
     */
    public static SciNotes.ActionOnDocument getActionOnDocument() {
        return new SciNotes.ActionOnDocument() {
            public void actionOn(ScilabDocument doc) throws IOException {
                TrailingWhiteManager trailingWhite = new TrailingWhiteManager(doc);
                trailingWhite.removeTrailingWhite(0, doc.getLength());
            }
        };
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
