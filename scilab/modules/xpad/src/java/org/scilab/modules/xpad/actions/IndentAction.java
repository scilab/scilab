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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class IndentAction extends DefaultAction {
	
	private static Xpad indent_editor;

	private IndentAction(Xpad editor) {
		super(XpadMessages.INDENT, editor);
		indent_editor = editor;
	}

	public void doAction() {
		ScilabStyleDocument styleDocument =  (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		
		try {
			styleDocument.beautifier(getEditor().getTextPane().getSelectionStart(), getEditor().getTextPane().getSelectionEnd());
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.INDENT, null, new IndentAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_I, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	public static void getXpadEditor(){
		((ScilabStyleDocument) indent_editor.getTextPane().getStyledDocument()).setEditor(indent_editor);
	}
}
