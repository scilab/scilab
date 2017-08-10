/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.scinotes.IndentManager;
import org.scilab.modules.scinotes.TabManager;

/**
 * IndentAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 *
 */
@SuppressWarnings("serial")
public final class IndentAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public IndentAction(String name, SciNotes editor) {
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
        IndentManager indent = sep.getIndentManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        int[] ret = new int[2];

        doc.mergeEditsBegin();
        if (start == end) {
            ret = indent.indentDoc(start, end);
            if (ret != null) {
                sep.setCaretPosition(ret[0]);
            }
        } else {
            ret = indent.indentDoc(start, end - 1);
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
                IndentManager indent = new IndentManager(doc);
                TabManager tab = new TabManager(doc, indent);
                tab.setDefaultTabulation();
                indent.indentDoc(0, doc.getLength());
            }
        };
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new IndentAction(label, editor), key);
    }
}
