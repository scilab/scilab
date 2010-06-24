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
     * @param editor SciNotes
     */
    private HelpOnKeywordAction(SciNotes editor) {
	super(SciNotesMessages.HELP_ON_KEYWORD, editor);
    }
    
    /**
     * doAction
     */
    public void doAction() {
	KeywordEvent kwe = ((ScilabEditorPane) getEditor().getTextPane()).getKeywordEvent();
	if (ScilabLexerConstants.isHelpable(kwe.getType())) {
	    try {
		String kw = getEditor().getTextPane().getDocument().getText(kwe.getStart(), kwe.getLength());
	    InterpreterManagement.requestScilabExec("help('" + kw + "')");    
	    } catch (BadLocationException e) { }
	}
    }
    
    /**
     * createMenu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	return createMenu(SciNotesMessages.HELP_ON_KEYWORD, null, new HelpOnKeywordAction(editor), key);
    }
}
