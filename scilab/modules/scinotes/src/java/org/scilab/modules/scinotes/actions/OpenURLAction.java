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

import java.awt.Desktop;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;

/**
 * OpenURLAction Class
 * @author Calixte DENIZET
 */
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
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent(sep.getSelectionEnd());
        if (ScilabLexerConstants.URL == kwe.getType() || ScilabLexerConstants.MAIL == kwe.getType()) {
            try {
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                String url = doc.getText(kwe.getStart(), kwe.getLength());
                openURL(url);
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
                public void propertyChange(PropertyChangeEvent arg0) {
                    String keyword = editor.getTextPane().getSelectedText();
                    if (keyword == null) {
                        KeywordEvent kwe = editor.getTextPane().getKeywordEvent();
                        menuitem.setEnabled(ScilabLexerConstants.URL == kwe.getType());
                    }
                }
            });

        return menuitem;
    }

    /**
     * @param url the url to open
     */
    public static void openURL(String url) {
        if (url == null || url.length() == 0) {
            return;
        }

        try {
            if (url.charAt(0) == 'h') {
                // We have something like http://...
                Desktop.getDesktop().browse(new URI(url));
            } else {
                // We have <pierre.marechal@scilab.org>
                String mail = "mailto:" + url.substring(1, url.length() - 1);
                Desktop.getDesktop().mail(new URI(mail));
            }
        } catch (IOException e) {
            System.err.println(e.toString());
        } catch (URISyntaxException e) {
            System.err.println(e.toString());
        }
    }
}
