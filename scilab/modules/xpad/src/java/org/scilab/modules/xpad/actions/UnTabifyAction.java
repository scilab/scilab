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

package org.scilab.modules.xpad.actions;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.TabManager;
import org.scilab.modules.xpad.utils.XpadMessages;
import org.scilab.modules.xpad.ScilabDocument;

/**
 * Class Untabify action for Xpad
 * @author Sylvestre Koumar
 *
 */
public class UnTabifyAction extends DefaultAction {
    
    /**
     * Default constructor
     * @param editor the editor
     */
    public UnTabifyAction(Xpad editor) {
	super(XpadMessages.UNTABIFY_SELECTION, editor);
    }
    
    /**
     * Function doAction
     */
    public void doAction() {
	ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
	int start = sep.getSelectionStart();
	int end   = sep.getSelectionEnd();
	TabManager tab = sep.getTabManager();
	ScilabDocument doc = (ScilabDocument) sep.getDocument();
	
	doc.mergeEditsBegin();
	if (start == end) {
	    tab.untabifyLine(start);
	} else {
	    int[] ret = tab.untabifyLines(start, end - 1);
	    if (ret != null) {
		sep.setSelectionStart(ret[0]);
		sep.setSelectionEnd(ret[1]);
	    }
	}
	doc.mergeEditsEnd();
    }
    
    /**
     * Create the MenuItem for untabify action
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
	return createMenu(XpadMessages.UNTABIFY_SELECTION , null, new UnTabifyAction(editor), key);
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param key KeyStroke
     * @param editor Editor
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
	textPane.getInputMap().put(key, new UnTabifyAction(editor));
    }
}
