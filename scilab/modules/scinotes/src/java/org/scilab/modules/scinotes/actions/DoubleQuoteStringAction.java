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

import java.io.IOException;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.CompoundUndoManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexer;
import org.scilab.modules.scinotes.ScilabLexerConstants;

/**
 * DoubleQuoteStringAction Class
 * @author Calixte DENIZET
 */
public final class DoubleQuoteStringAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public DoubleQuoteStringAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        doc.getUndoManager().enableOneShot(true);
        doc.mergeEditsBegin();
        doubleQuoteString(doc);
        doc.mergeEditsEnd();
        doc.getUndoManager().enableOneShot(false);
    }

    /**
     * Replace the strings simply quoted by double quoted strings
     * @param doc the documen
     */
    public static void doubleQuoteString(ScilabDocument doc) {
        StringBuffer buffer = new StringBuffer(128);
        ScilabLexer lexer = doc.createLexer(false);
        Element elem = doc.getDefaultRootElement();
        Element line;
        int lastLine = elem.getElementIndex(doc.getLength());
        int tok;

        try {
            for (int i = 0; i <= lastLine; i++) {
                line = elem.getElement(i);
                lexer.setRange(line.getStartOffset(), line.getEndOffset());
                do {
                    tok = lexer.scan();
                    if (ScilabLexerConstants.isString(tok)) {
                        buffer.append(doc.getText(lexer.start + lexer.yychar(), lexer.yylength()));
                    } else if (buffer.length() > 0) {
                        boolean modified = false;
                        int len = buffer.length();
                        if (buffer.charAt(0) == '\'') {
                            buffer.replace(0, 1, "\"");
                            modified = true;
                        }
                        if (len > 1 && buffer.charAt(len - 1) == '\'') {
                            buffer.replace(len - 1, len, "\"");
                            modified = true;
                        }
                        if (modified) {
                            int start = lexer.start + lexer.yychar() - len;
                            doc.replace(start, len, buffer.toString(), null);
                        }
                        buffer.setLength(0);
                    }
                } while (tok != ScilabLexerConstants.EOF);
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    /**
     * @return an action to replace single quoted strings by double quoted ones in a document
     */
    public static SciNotes.ActionOnDocument getActionOnDocument() {
        return new SciNotes.ActionOnDocument() {
            public void actionOn(ScilabDocument doc) throws IOException {
                doubleQuoteString(doc);
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
        return createMenu(label, null, new DoubleQuoteStringAction(label, editor), key);
    }
}
