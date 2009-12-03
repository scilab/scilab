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

public class CommentAction extends DefaultAction {

	private CommentManager commentManager = new CommentManager();
	
	private CommentAction(Xpad editor) {
		super(XpadMessages.COMMENT_SELECTION, editor);
	}
	
	public void doAction()
	{
		ScilabStyleDocument doc = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		synchronized(doc){
			int position_start = getEditor().getTextPane().getSelectionStart();
			int position_end   = getEditor().getTextPane().getSelectionEnd();
			
			int line_start     = doc.getDefaultRootElement().getElementIndex(position_start);
			int line_end       = doc.getDefaultRootElement().getElementIndex(position_end);
			
			if(position_start == position_end)
			{
				// No selection : comment the current line
				int offset = commentManager.commentLine(doc, line_start);
				getEditor().getTextPane().setCaretPosition(position_start+offset);
			}
			
			else if( line_start == line_end )
			{
				// A part of the line is selected
				int offset = commentManager.commentText(doc, position_start);
				getEditor().getTextPane().setSelectionStart(position_start);
				getEditor().getTextPane().setSelectionEnd(position_end+offset);
			}
			
			else
			{
				// several lines are selected
				commentManager.commentLines(doc, line_start, line_end);
				position_end = doc.getDefaultRootElement().getElement(line_end).getEndOffset();
				
				getEditor().getTextPane().setSelectionStart(position_start);
				getEditor().getTextPane().setSelectionEnd(position_end-1);
			}

		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.COMMENT_SELECTION , null, new CommentAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_D, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
