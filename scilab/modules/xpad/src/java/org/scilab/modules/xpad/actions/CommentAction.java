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

import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class CommentAction extends DefaultAction {

	private CommentAction(Xpad editor) {
		super(XpadMessages.COMMENT_SELECTION, editor);
	}
	
	public void doAction()
	{
		int position_start = getEditor().getTextPane().getSelectionStart();
		int position_end   = getEditor().getTextPane().getSelectionEnd();
		
		int line_start     = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_start);
		int line_end       = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_end);
		
		if(position_start == position_end)
		{
			// No selection : comment the current line
			int offset = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).commentLine(line_start);
			getEditor().getTextPane().setCaretPosition(position_start+offset);
		}
		
		else if( line_start == line_end )
		{
			// A part of the line is selected
			int offset = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).commentText(position_start);
			getEditor().getTextPane().setSelectionStart(position_start);
			getEditor().getTextPane().setSelectionEnd(position_end+offset);
		}
		
		else
		{
			// several lines are selected
			((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).commentLines(line_start, line_end);
			position_end = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElement(line_end).getEndOffset();
			
			getEditor().getTextPane().setSelectionStart(position_start);
			getEditor().getTextPane().setSelectionEnd(position_end-1);
		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.COMMENT_SELECTION , null, new CommentAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_D, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
