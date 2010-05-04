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
 * Class Tabify action for Xpad
 * @author Sylvestre Koumar
 *
 */
public final class TabifyAction extends DefaultAction {

	/**
	 * Default constructor
	 * @param editor the editor
	 */
	private TabifyAction(Xpad editor) {
	    super(XpadMessages.TABIFY_SELECTION, editor);
	}
	
	/**
	 * Function doAction
	 */
	public synchronized void doAction() {
	    ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
	    int start = sep.getSelectionStart();
	    int end   = sep.getSelectionEnd();
	    TabManager tab = sep.getTabManager();
	    ScilabDocument doc = (ScilabDocument) sep.getDocument();
		
	    doc.mergeEditsBegin();
	    if (start == end) {
		tab.insertTab(start);
	    } else {
		int[] ret = tab.tabifyLines(start, end - 1);
		if (ret != null) {
		    sep.setSelectionStart(ret[0]);
		    sep.setSelectionEnd(ret[1]);
		}
	    }
	    doc.mergeEditsEnd();
	}
	
	/**
	 * Create the MenuItem for tabify action
	 * @param editor Editor
	 * @param key KeyStroke
	 * @return a MenuItem
	 */
        public static MenuItem createMenu(Xpad editor, KeyStroke key) {
		return createMenu(XpadMessages.TABIFY_SELECTION , null, new TabifyAction(editor), key);
	}

        /**
	 * Put input map
	 * @param textPane JTextpane
	 * @param editor Editor
	 * @param key KeyStroke
	 */
        public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
	    textPane.getInputMap().put(key, new TabifyAction(editor));
	}
}
