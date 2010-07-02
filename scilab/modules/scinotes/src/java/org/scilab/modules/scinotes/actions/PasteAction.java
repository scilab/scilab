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

import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

public class PasteAction extends DefaultAction {

    private PasteAction(SciNotes editor) {
	super(SciNotesMessages.PASTE, editor);
    }
    
    public void doAction() {
	ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
	doc.mergeEditsBegin();
	getEditor().getTextPane().getActionMap().get(DefaultEditorKit.pasteAction).actionPerformed(null);
	doc.mergeEditsEnd();
    }

    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	return createMenu(SciNotesMessages.PASTE, null, new PasteAction(editor), key);
    }
    
    public static PushButton createButton(SciNotes editor) {
	return createButton(SciNotesMessages.PASTE, "edit-paste.png", new PasteAction(editor));
    }
    
    /**
     * Put input map
     * @param textPane JTextpane
     * @param key KeyStroke
     * @param editor Editor
     */
    public static void putInInputMap(JComponent textPane, SciNotes editor, KeyStroke key) {
	textPane.getInputMap().put(key, new PasteAction(editor));
    }
}
