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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.StringTokenizer;

import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * HelpOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class HelpOnKeywordAction extends DefaultAction {

    private static final long serialVersionUID = -7241378949942315933L;

    protected boolean isPopup;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public HelpOnKeywordAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        String selection = "";
        int start = getEditor().getTextPane().getSelectionStart();
        int end = getEditor().getTextPane().getSelectionEnd();
        try {
            if (start == end) {
                String kw = getEditor().getTextPane().getHelpableKeyword(!isPopup);
                if (kw != null) {
                    selection = kw;
                }
            } else {
                selection = getEditor().getTextPane().getDocument().getText(start, end - start);
            }
        } catch (BadLocationException e) { }

        InterpreterManagement.requestScilabExec("help('" + selection + "')");
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
        return createMenu(label1, label2, editor, key, new HelpOnKeywordAction(label1 + SciNotesMessages.DOTS, editor));
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @param hoka the HelpOnKeyword action
     * @return MenuItem
     */
    protected static MenuItem createMenu(final String label1, final String label2, final SciNotes editor, KeyStroke key, final HelpOnKeywordAction hoka) {
        final MenuItem menuitem = createMenu(label1, null, hoka, key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    String select = editor.getTextPane().getSelectedText();
                    if (select == null) {
                        String kw = editor.getTextPane().getHelpableKeyword(!hoka.isPopup);
                        if (kw != null) {
                            menuitem.setText(label1 + SciNotesMessages.QUOTE + kw + SciNotesMessages.QUOTE);
                            menuitem.setEnabled(true);
                        } else {
                            menuitem.setText(label1 + SciNotesMessages.DOTS);
                            menuitem.setEnabled(false);
                        }
                    } else {
                        menuitem.setText(label2);
                        menuitem.setEnabled(true);
                    }
                }
            }
        });

        return menuitem;
    }
}
