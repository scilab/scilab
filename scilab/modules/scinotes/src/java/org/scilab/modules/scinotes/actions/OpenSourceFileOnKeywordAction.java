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
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * OpenSourceFileOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class OpenSourceFileOnKeywordAction extends DefaultAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private OpenSourceFileOnKeywordAction(SciNotes editor) {
        super(SciNotesMessages.OPEN_SOURCE_FILE_ON_KEYWORD, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent();
        if (ScilabLexerConstants.isOpenable(kwe.getType())) {
            try {
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                String kw = doc.getText(kwe.getStart(), kwe.getLength());
                int pos = doc.searchFunctionByName(kw);
                if (pos != -1) {
                    sep.scrollTextToPos(pos);
                } else {
                    String path = "get_function_path('" + kw + "')";
                    InterpreterManagement.requestScilabExec("if " + path +" ~=[] then editor(" + path + ");end");
                }
            } catch (BadLocationException e) { }
        }
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.OPEN_SOURCE_FILE_ON_KEYWORD, null, new OpenSourceFileOnKeywordAction(editor), key);
    }
}
