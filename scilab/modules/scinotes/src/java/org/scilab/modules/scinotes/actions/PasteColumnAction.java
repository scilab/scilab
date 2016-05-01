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

import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.StringReader;
import java.util.StringTokenizer;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.CompoundUndoManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesCaret;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorKit;

/**
 * Class for paste action
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class PasteColumnAction extends DefaultAction {

    private static final String CR = "\n";

    /**
     * Constructor
     * @param name the name of the action
     * @param editor to associate
     */
    public PasteColumnAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Action !
     */
    public void doAction() {
        ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
        Element root = doc.getDefaultRootElement();
        String str = null;
        try {
            str = (String) Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this).getTransferData(DataFlavor.stringFlavor);
            if (str != null) {
                str = ((ScilabEditorKit) getEditor().getEditorKit()).read(new StringReader(str)).content;
            }
        } catch (UnsupportedFlavorException ex1) {
            System.err.println(ex1);
        } catch (IOException ex2) {
            System.err.println(ex2);
        }
        if (str != null) {
            SciNotesCaret caret = (SciNotesCaret) getEditor().getTextPane().getCaret();
            int pos;
            if (caret.isEmptySelection()) {
                pos = getEditor().getTextPane().getSelectionStart();
            } else {
                int[][] positions = caret.getSelectedPositions();
                pos = positions[0][0];
            }
            int spos = pos;
            doc.getUndoManager().enableOneShot(true);
            doc.mergeEditsBegin();
            getEditor().getTextPane().replaceSelection("");
            StringTokenizer tokens = new StringTokenizer(str, CR);
            int index = root.getElementIndex(pos);
            String crs = initString(tokens.countTokens() - root.getElementCount() + index, '\n');
            if (crs.length() > 0) {
                try {
                    doc.insertString(doc.getLength(), crs, null);
                } catch (BadLocationException e) { }
            }
            int col = pos - root.getElement(index).getStartOffset();
            while (tokens.hasMoreTokens()) {
                String tok = tokens.nextToken();
                int end = root.getElement(index).getEndOffset() - 1;
                if (pos > end) {
                    tok = initString(pos - end, ' ') + tok;
                    pos = end;
                }
                try {
                    doc.insertString(pos, tok, null);
                } catch (BadLocationException e) { }
                if (index < root.getElementCount() - 1) {
                    pos = root.getElement(++index).getStartOffset() + col;
                }
            }
            doc.mergeEditsEnd();
            doc.getUndoManager().enableOneShot(false);
            getEditor().getTextPane().setCaretPosition(spos);
        }
    }

    /**
     * Init a string
     * @param n the length of the string
     * @param c the char to use to initialize
     * @return the initialized string
     */
    private static String initString(int n, char c) {
        if (n > 0) {
            char[] buf = new char[n];
            for (int i = 0; i < n; i++) {
                buf[i] = c;
            }

            return new String(buf);
        }
        return "";
    }

    /**
     * Create the MenuItem for print action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new PasteColumnAction(label, editor), key);
    }
}
