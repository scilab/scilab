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
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotesCaret;
import org.scilab.modules.scinotes.TabManager;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * Class Untabify action for SciNotes
 * @author Sylvestre Koumar
 *
 */
@SuppressWarnings(value = { "serial" })
public class UnTabifyAction extends DefaultAction {

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor the editor
     */
    public UnTabifyAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Function doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        int start = sep.getSelectionStart();
        int end   = sep.getSelectionEnd();
        int posC = sep.getCaretPosition();
        TabManager tab = sep.getTabManager();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();

        doc.mergeEditsBegin();
        if (((SciNotesCaret) sep.getCaret()).isEmptySelection()) {
            if (start == end) {
                tab.untabifyLine(start);
            } else {
                int[] ret = tab.untabifyLines(start, end - 1);
                if (ret != null) {
                    if (posC == start) {
                        sep.select(ret[1], ret[0]);
                    } else {
                        sep.select(ret[0], ret[1]);
                    }
                }
            }
        } else {
            int[][] pos = ((SciNotesCaret) sep.getCaret()).getSelectedPositions();
            ((SciNotesCaret) sep.getCaret()).protectHighlights(true);
            int ret;
            int sret = 0;
            for (int i = 0; i < pos.length; i++) {
                if (pos[i][0] < pos[i][1]) {
                    ret = tab.removeTab(pos[i][0] - sret);
                    sret += ret;
                    pos[i][0] -= sret;
                    pos[i][1] -= sret;
                }
            }
            ((SciNotesCaret) sep.getCaret()).protectHighlights(false);
            ((SciNotesCaret) sep.getCaret()).updateHighlights();
        }

        doc.mergeEditsEnd();
    }

    /**
     * Create the MenuItem for untabify action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new UnTabifyAction(label, editor), key);
    }
}
