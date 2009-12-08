/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
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
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.CommentManager;
import org.scilab.modules.xpad.style.ColorizationManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class UnCommentAction extends DefaultAction {

	private CommentManager commentManager = new CommentManager();
	
	private UnCommentAction(Xpad editor)
	{
		super(XpadMessages.UNCOMMENT_SELECTION, editor);
	}
	
	public void doAction()
	{
		int position_start = getEditor().getTextPane().getSelectionStart();
		int position_end   = getEditor().getTextPane().getSelectionEnd();
		ScilabStyleDocument scilabDocument = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		int line_start     = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_start);
		int line_end       = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_end);
		
		if(position_start == position_end)
		{
			// No selection : uncomment the current line
			int offset = commentManager.uncommentLine(scilabDocument, line_start);
			getEditor().getTextPane().setCaretPosition(position_start-offset);
		}
		else if( line_start == line_end )
		{
			// A part of the line is selected
			int offset = commentManager.uncommentText(scilabDocument, position_start);
			getEditor().getTextPane().setSelectionStart(position_start);
			getEditor().getTextPane().setSelectionEnd(position_end-offset);
		}
		else
		{
			// several lines are selected
			commentManager.uncommentLines(scilabDocument, line_start, line_end);
			position_end = scilabDocument.getDefaultRootElement().getElement(line_end).getEndOffset();
			
			getEditor().getTextPane().setSelectionStart(position_start);
			getEditor().getTextPane().setSelectionEnd(position_end-1);
		}
		SwingUtilities.invokeLater(new ColorizationManager().new ColorUpdater(scilabDocument, position_start, position_end) );
	}
	
	public static MenuItem createMenu(Xpad editor)
	{
		return createMenu(XpadMessages.UNCOMMENT_SELECTION, null, new UnCommentAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_D, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()+ActionEvent.SHIFT_MASK));
	}
}
