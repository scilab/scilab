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

package org.scilab.modules.xpad.actions;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabDocument;
import org.scilab.modules.xpad.ScilabLexerConstants;
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.KeywordEvent;
import org.scilab.modules.xpad.utils.XpadMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * OpenSourceFileOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class OpenSourceFileOnKeywordAction extends DefaultAction {

    /**
     * Constructor
     * @param editor Xpad
     */
    private OpenSourceFileOnKeywordAction(Xpad editor) {
        super(XpadMessages.OPEN_SOURCE_FILE_ON_KEYWORD, editor);
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
                    sep.getScrollPane().getVerticalScrollBar().setValue(sep.modelToView(pos).y);
                    sep.setCaretPosition(pos);
                } else {
                InterpreterManagement.requestScilabExec("a46d43fa4w5z8512dc7dc2c3=get_function_path('" + kw + "');if a46d43fa4w5z8512dc7dc2c3~=[] then editor(a46d43fa4w5z8512dc7dc2c3);clear a46d43fa4w5z8512dc7dc2c3;end");
                }
            } catch (BadLocationException e) { }
        }
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.OPEN_SOURCE_FILE_ON_KEYWORD, null, new OpenSourceFileOnKeywordAction(editor), key);
    }
}
