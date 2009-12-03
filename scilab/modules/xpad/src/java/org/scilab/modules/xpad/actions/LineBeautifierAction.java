/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
import java.awt.event.ActionEvent;
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.JTextPane;
import org.scilab.modules.xpad.ScilabEditorKit;


public class LineBeautifierAction extends ScilabEditorKit.InsertBreakAction {
	
	private IndentManager indentManager = new IndentManager();
	
	public void actionPerformed( ActionEvent ev) {
		super.actionPerformed(ev);
		JTextPane textPane = (JTextPane) ev.getSource();
		ScilabStyleDocument doc =  (ScilabStyleDocument)textPane.getStyledDocument();
		javax.swing.text.Element root = doc.getDefaultRootElement();
		int newLine =  root.getElementIndex(textPane.getCaretPosition());
		int startOfPreviousLine = root.getElement((newLine > 1) ? newLine - 2 : newLine - 1).getStartOffset();
		int endOfNewLine = root.getElement(newLine).getEndOffset();// should be textPane.getCaretPosition()
//		System.err.println("doc.isUpdater()"+doc.isUpdater()+ "doc.getAutoIndent()"+doc.getAutoIndent());
		if (doc.isUpdater() && doc.getAutoIndent()) {
			//System.err.println("startOfPreviousLine:"+startOfPreviousLine+ "endOfNewLine:"+endOfNewLine);
			boolean autoColorize= doc.getAutoColorize();
			doc.setAutoColorize(false);
			doc.disableUndoManager();
			try {
				//
				indentManager.beautifier(doc,startOfPreviousLine, endOfNewLine);
				// hard to compute safe start offset :(
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} finally {
				doc.setAutoColorize(autoColorize);
			}
			doc.enableUndoManager();
		}
	}
	public static void putInInputMap(JComponent textPane) {
		textPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,0), new LineBeautifierAction());
		return;
	}
}
