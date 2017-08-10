/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
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

import java.util.regex.Pattern;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * NextParagraphAction Class
 * @author Calixte DENIZET
 */
public class NextParagraphAction extends DefaultAction {

    private static final long serialVersionUID = 1L;
    private static final Pattern pat = Pattern.compile("[ \\t\\r\\n]*");
    protected int next = +1;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public NextParagraphAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        Element root = doc.getDefaultRootElement();
        int line = root.getElementIndex(sep.getCaretPosition());
        boolean empty = isEmptyLine(doc, line);
        int i = line + next;
        int stop = next > 0 ? root.getElementCount() : -1;
        for (; i != stop; i += next) {
            boolean b = isEmptyLine(doc, i);
            if (empty && !b) {
                empty = false;
            } else if (!empty && b) {
                break;
            }
        }
        if (i == stop) {
            i = next > 0 ? stop - 1 : 0;
        }

        sep.setCaretPosition(root.getElement(i).getStartOffset());
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new NextParagraphAction(label, editor), key);
    }

    /**
     * Test if a lin e is empty
     * @param doc the document
     * @param line the line number
     * @return true if the line is empty
     */
    private static final boolean isEmptyLine(ScilabDocument doc, int line) {
        try {
            Element e = doc.getDefaultRootElement().getElement(line);
            int s = e.getStartOffset();
            String str = doc.getText(s, e.getEndOffset() - s);
            return pat.matcher(str).matches();
        } catch (BadLocationException e) { }

        return false;
    }
}
