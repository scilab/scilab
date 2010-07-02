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
import javax.swing.JComponent;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * CutAction Class
 * @author Bruno JOFRET
 *
 */
public final class CutAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -4831313579986185630L;
    
    /**
     * Constructor 
     * @param editor SciNotes
     */
    private CutAction(SciNotes editor) {
	super(SciNotesMessages.CUT, editor);
    }
    
    /**
     * doAction
     */
    public void doAction() {
	ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
	doc.mergeEditsBegin();
	getEditor().getTextPane().getActionMap().get(DefaultEditorKit.cutAction).actionPerformed(null);
	doc.mergeEditsEnd();
    }

    /**
     * createMenu
     * @param editor SciNotes 
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	return createMenu(SciNotesMessages.CUT, null, new CutAction(editor), key);
    }
    
    /**
     * createButton
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(SciNotes editor) {
	return createButton(SciNotesMessages.CUT, "edit-cut.png", new CutAction(editor));
    }
    
    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, SciNotes editor, KeyStroke key) {
	textPane.getInputMap().put(key, new CutAction(editor));
    }
}
