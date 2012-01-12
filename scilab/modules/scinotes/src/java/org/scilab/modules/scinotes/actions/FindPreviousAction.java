/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.util.List;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SearchManager;

/**
 *
 * @author Antoine ELIAS
 *
 */
public final class FindPreviousAction extends DefaultAction {

    private static final long serialVersionUID = -9017015781643180532L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public FindPreviousAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        try {
            ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
            int startPos = getEditor().getTextPane().getSelectionStart();
            int endPos = getEditor().getTextPane().getSelectionEnd();
            int startLine = doc.getDefaultRootElement().getElementIndex(startPos);
            int endLine = doc.getDefaultRootElement().getElementIndex(endPos);

            //don't manage multiple lines quick find
            if (startLine != endLine) {
                return;
            }

            String exp = null;
            if (startPos == endPos) {
                //nothing to search
                if (FindAction.getPreviousSearch() == null) {
                    return;
                } else {
                    exp = FindAction.getPreviousSearch();
                }
            } else {
                exp = doc.getText(startPos, endPos - startPos);
            }

            //search from current position to end document
            List<Integer[]> offsets = SearchManager.findWord(doc, exp, 0, doc.getLength(), true, false, false);
            if (offsets.size() != 0) {
                int index = -1;
                for (int i = 0; i < offsets.size(); i++) {
                    if (offsets.get(i)[0] >= startPos) {
                        index = i - 1;
                        break;
                    }
                }

                if (index == -1) {
                    index = offsets.size() - 1;
                }

                getEditor().getTextPane().select(offsets.get(index)[0], offsets.get(index)[1]);
            }
        } catch (BadLocationException e) {
            e.printStackTrace();
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
        return createMenu(label, null, new FindPreviousAction(label, editor), key);
    }
}
