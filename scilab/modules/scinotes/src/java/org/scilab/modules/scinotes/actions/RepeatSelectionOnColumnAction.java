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

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * Class for paste action
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class RepeatSelectionOnColumnAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor to associate
     */
    public RepeatSelectionOnColumnAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Action !
     */
    public void doAction() {
        ScilabEditorPane pane = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
        Element root = doc.getDefaultRootElement();
        String str = null;
        boolean select = false;

        if (pane.getSelectionStart() == pane.getSelectionEnd()) {
            try {
                str = doc.getText(pane.getCaretPosition(), 1);
            } catch (BadLocationException e) { }
        } else {
            str = pane.getSelectedText();
            select = true;
        }

        if (str != null) {
            doc.mergeEditsBegin();
            int pos = pane.getSelectionStart();
            int index = root.getElementIndex(pos);
            int col = pos - root.getElement(index).getStartOffset();
            int len = str.length();
            String cr = "";
            int end;
            int spos = 0;
            if (index < root.getElementCount() - 1) {
                pos = root.getElement(index + 1).getStartOffset() + col;
                end = root.getElement(index + 1).getEndOffset() - 1;
            } else {
                cr = "\n";
                spos = 1;
                end = root.getElement(index).getEndOffset() - 1;
                pos = end + col;
            }

            spos += pos;
            if (pos >= end) {
                str = cr + whites(pos - end) + str;
                pos = end;
            }
            try {
                doc.insertString(pos, str, null);
            } catch (BadLocationException e) { }

            doc.mergeEditsEnd();
            if (select) {
                pane.select(spos, spos + len);
            } else {
                pane.setCaretPosition(spos);
            }
        }
    }

    /**
     * Generates a string of whites
     * @param n the whites number
     * @param the string
     */
    private static String whites(int n) {
        if (n > 0) {
            char[] buf = new char[n];
            for (int i = 0; i < n; i++) {
                buf[i] = ' ';
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
        return createMenu(label, null, new RepeatSelectionOnColumnAction(label, editor), key);
    }
}
