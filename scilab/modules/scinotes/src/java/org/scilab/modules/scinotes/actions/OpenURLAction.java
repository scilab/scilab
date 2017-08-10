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
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;

/**
 * OpenURLAction Class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class OpenURLAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public OpenURLAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent(sep.getSelectionEnd());
        if (ScilabLexerConstants.URL == kwe.getType() || ScilabLexerConstants.MAIL == kwe.getType()) {
            try {
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                String url = doc.getText(kwe.getStart(), kwe.getLength());
                WebBrowser.openUrl(url);
            } catch (BadLocationException e) { }
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        final MenuItem menuitem = createMenu(label, null, new OpenURLAction(label, editor), key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    String keyword = editor.getTextPane().getSelectedText();
                    if (keyword == null) {
                        KeywordEvent kwe = editor.getTextPane().getKeywordEvent();
                        menuitem.setEnabled(ScilabLexerConstants.MAIL == kwe.getType() || ScilabLexerConstants.URL == kwe.getType());
                    }
                }
            }
        });

        return menuitem;
    }
}
