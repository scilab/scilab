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
public final class FindNextAction extends DefaultAction {

    private static final long serialVersionUID = -9017015781643180532L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public FindNextAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        try {
            int startPos = getEditor().getTextPane().getSelectionStart();
            int endPos = getEditor().getTextPane().getSelectionEnd();
            int startLine = ((ScilabDocument) getEditor().getTextPane().getDocument()).getDefaultRootElement().getElementIndex(startPos);
            int endLine = ((ScilabDocument) getEditor().getTextPane().getDocument()).getDefaultRootElement().getElementIndex(endPos);

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
                exp = getEditor().getTextPane().getDocument().getText(startPos, endPos - startPos);
            }

            ScilabDocument scilabStyle = (ScilabDocument) getEditor().getTextPane().getDocument();

            //search from current position to end document
            List<Integer[]> offsets = SearchManager.findWord(scilabStyle, exp, 0, scilabStyle.getLength(), false, false, false);
            if (offsets.size() != 0) {
                int index = -1;
                for (int i = 0; i < offsets.size(); i++) {
                    if (offsets.get(i)[0] >= endPos + 1) {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    index = 0;
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
        return createMenu(label, null, new FindNextAction(label, editor), key);
    }
}
