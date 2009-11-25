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

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;
import org.scilab.modules.xpad.style.IndentManager;

@SuppressWarnings("serial")
public class IndentAction extends DefaultAction {
	
	private IndentManager indentManager = new IndentManager();
	
	private IndentAction(Xpad editor) {
		super(XpadMessages.INDENT, editor);
	}

	public void doAction() {
		ScilabStyleDocument styleDocument =  (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		
		try {
			int selection_start = getEditor().getTextPane().getSelectionStart();
			int selection_end = getEditor().getTextPane().getSelectionEnd();
			int final_selection_end = indentManager.beautifier(styleDocument, selection_start, selection_end);
			getEditor().getTextPane().setSelectionStart(selection_start);
			getEditor().getTextPane().setSelectionEnd(final_selection_end);
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.INDENT, null, new IndentAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_I, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	public static void getXpadEditor(){

	}
}
