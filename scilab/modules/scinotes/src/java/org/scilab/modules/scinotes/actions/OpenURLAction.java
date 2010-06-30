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
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * OpenURLAction Class
 * @author Calixte DENIZET
 */
public class OpenURLAction extends DefaultAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private OpenURLAction(SciNotes editor) {
        super(SciNotesMessages.OPEN_URL, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent(sep.getSelectionEnd());
        if (ScilabLexerConstants.URL == kwe.getType()) {
            try {
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                String url = doc.getText(kwe.getStart(), kwe.getLength());
		Desktop.getDesktop().browse(new URI(url));
            } catch (BadLocationException e) { }
	    catch (IOException e) { 
		System.err.println(e.toString());
	    }
	    catch (URISyntaxException e) {
		System.err.println(e.toString());
	    }
        }
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.OPEN_URL, null, new OpenURLAction(editor), key);
    }
}
