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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.StringTokenizer;

import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.CompoundUndoManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;

/**
 * CapitalizeAction Class
 * @author Calixte DENIZET
 */
public final class CapitalizeAction extends DefaultAction {

    private static final long serialVersionUID = 1531881688147993338L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CapitalizeAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        String str;
        int start = sep.getSelectionStart();
        int end = sep.getSelectionEnd();

        if (start == end) {
            try {
                String letter = doc.getText(start, 1);
                char c = letter.charAt(0);
                if (Character.isUpperCase(c)) {
                    str = letter.toLowerCase();
                } else {
                    str = letter.toUpperCase();
                }
                doc.mergeEditsBegin();
                doc.getUndoManager().enableOneShot(true);
                sep.select(start, start + 1);
                sep.replaceSelection(str);
                doc.mergeEditsEnd();
                doc.getUndoManager().enableOneShot(false);
            } catch (BadLocationException e) { }
        } else {
            str = sep.getSelectedText();
            boolean nonLetter = true;
            char[] buf = str.toCharArray();
            for (int i = 0; i < buf.length; i++) {
                char c = buf[i];
                if (Character.isLetter(c)) {
                    if (nonLetter) {
                        if (Character.isLowerCase(c)) {
                            buf[i] = Character.toUpperCase(c);
                        } else {
                            buf[i] = Character.toLowerCase(c);
                        }
                        nonLetter = false;
                    }
                } else {
                    nonLetter = true;
                }
            }

            doc.mergeEditsBegin();
            sep.replaceSelection(new String(buf));
            sep.select(start, end);
            doc.mergeEditsEnd();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, ";\uff1b");
        final String label1 = token.nextToken();
        final String label2 = token.nextToken();
        return createMenu(label1, label2, editor, key, new CapitalizeAction(label1, editor));
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @param hoka the HelpOnKeyword action
     * @return MenuItem
     */
    protected static MenuItem createMenu(final String label1, final String label2, final SciNotes editor, KeyStroke key, final CapitalizeAction hoka) {
        final MenuItem menuitem = createMenu(label1, null, hoka, key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    String select = editor.getTextPane().getSelectedText();
                    if (select == null) {
                        menuitem.setText(label2);
                    } else {
                        menuitem.setText(label1);
                    }
                }
            }
        });

        return menuitem;
    }
}
