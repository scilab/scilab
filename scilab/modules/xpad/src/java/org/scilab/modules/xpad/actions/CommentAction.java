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

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.CommentManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;
/**
 * CommentAction Class
 * @author Bruno JOFRET
 *
 */
public final class CommentAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -7258307088402814986L;
	private CommentManager commentManager = new CommentManager();
	
	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private CommentAction(Xpad editor) {
		super(XpadMessages.COMMENT_SELECTION, editor);
	}
	
	/**
	 * doAction
	 */
	public void doAction() {
		ScilabStyleDocument doc = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		synchronized (doc) {
			int positionStart = getEditor().getTextPane().getSelectionStart();
			int positionEnd   = getEditor().getTextPane().getSelectionEnd();
			
			int lineStart     = doc.getDefaultRootElement().getElementIndex(positionStart);
			int lineEnd       = doc.getDefaultRootElement().getElementIndex(positionEnd);
			
			if (positionStart == positionEnd) {
				// No selection : comment the current line
				int offset = commentManager.commentLine(doc, lineStart);
				getEditor().getTextPane().setCaretPosition(positionStart + offset);
			} else if (lineStart == lineEnd) {
				// A part of the line is selected
				int offset = commentManager.commentText(doc, positionStart);
				getEditor().getTextPane().setSelectionStart(positionStart);
				getEditor().getTextPane().setSelectionEnd(positionEnd + offset);
			} else {
				// several lines are selected
				commentManager.commentLines(doc, lineStart, lineEnd);
				positionEnd = doc.getDefaultRootElement().getElement(lineEnd).getEndOffset();
				
				getEditor().getTextPane().setSelectionStart(positionStart);
				getEditor().getTextPane().setSelectionEnd(positionEnd - 1);
			}
		}
	}
	
	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.COMMENT_SELECTION, null, 
				new CommentAction(editor), 
				KeyStroke.getKeyStroke(KeyEvent.VK_D, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
 