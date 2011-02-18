/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.scinotes.SciNotesCaret;
import org.scilab.modules.scinotes.TabManager;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * Class Tabify action for SciNotes
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 *
 */
public final class TabifyAction extends DefaultAction {

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor the editor
     */
    public TabifyAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Function doAction
     */
    public synchronized void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end   = sep.getSelectionEnd();
        TabManager tab = sep.getTabManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();

        doc.mergeEditsBegin();
        if (((SciNotesCaret) sep.getCaret()).isEmptySelection()) {
            if (start == end) {
                tab.insertTab(start);
            } else {
                int[] ret = tab.tabifyLines(start, end - 1);
                if (ret != null) {
                    sep.setSelectionStart(ret[0]);
                    sep.setSelectionEnd(ret[1]);
                }
            }
        } else {
            int[][] pos = ((SciNotesCaret) sep.getCaret()).getSelectedPositions();
            ((SciNotesCaret) sep.getCaret()).protectHighlights(true);
            int ret;
            int sret = 0;
            for (int i = 0; i < pos.length; i++) {
                if (pos[i][0] < pos[i][1]) {
                    ret = tab.insertTab(pos[i][0] + sret);
                    sret += ret;
                    pos[i][0] += sret;
                    pos[i][1] += sret;
                }
            }
            ((SciNotesCaret) sep.getCaret()).protectHighlights(false);
            ((SciNotesCaret) sep.getCaret()).updateHighlights();
        }
        doc.mergeEditsEnd();
    }

    /**
     * Create the MenuItem for tabify action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new TabifyAction(label, editor), key);
    }
}
