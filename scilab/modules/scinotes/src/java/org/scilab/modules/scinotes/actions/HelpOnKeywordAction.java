/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * HelpOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class HelpOnKeywordAction extends DefaultAction {

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
                KeywordEvent kwe = ((ScilabEditorPane) getEditor().getTextPane()).getKeywordEvent();
                if (ScilabLexerConstants.isHelpable(kwe.getType())) {
                    selection = getEditor().getTextPane().getDocument().getText(kwe.getStart(), kwe.getLength());
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
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        StringTokenizer token = new StringTokenizer(label, ";");
        final String label1 = token.nextToken();
        final String label2 = token.nextToken();
        final MenuItem menuitem = createMenu(label1, null, new HelpOnKeywordAction(label1 + SciNotesMessages.DOTS, editor), key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    String select = editor.getTextPane().getSelectedText();
                    if (select == null) {
                        KeywordEvent kwe = ((ScilabEditorPane) editor.getTextPane()).getKeywordEvent();
                        if (ScilabLexerConstants.isHelpable(kwe.getType())) {
                            try {
                                String kw = editor.getTextPane().getDocument().getText(kwe.getStart(), kwe.getLength());
                                menuitem.setText(label1 + SciNotesMessages.QUOTE + kw + SciNotesMessages.QUOTE);
                                menuitem.setEnabled(true);
                            } catch (BadLocationException ex) { }
                        } else {
                            menuitem.setText(label1 + SciNotesMessages.DOTS);
                            menuitem.setEnabled(false);
                        }
                    } else {
                        menuitem.setText(label2);
                        menuitem.setEnabled(true);
                    }
                }
            });

        return menuitem;
    }
}
