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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class UnTabifyAction extends DefaultAction {

	private UnTabifyAction(Xpad editor) {
	  super(XpadMessages.UNTABIFY_SELECTION, editor);
	}
	
	public void doAction()
	{
		int position_start = getEditor().getTextPane().getSelectionStart();
		int position_end   = getEditor().getTextPane().getSelectionEnd();
		
		int line_start     = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_start);
		int line_end       = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(position_end);
		 if( line_start == line_end )
		{
			// A part of the line is selected : Delete a Tab at the beginning of the line
			int offset = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).untabifyLine(line_start);
			getEditor().getTextPane().setSelectionStart(position_start-offset);
			getEditor().getTextPane().setSelectionEnd(position_end-offset);
		}
		
		else
		{
			// several lines are selected
			int offset = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).untabifyLines(line_start, line_end);
			getEditor().getTextPane().setSelectionStart(position_start-offset);
			getEditor().getTextPane().setSelectionEnd(position_end - offset*(line_end-line_start+1));
		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.UNTABIFY_SELECTION , null, new UnTabifyAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_TAB,ActionEvent.SHIFT_MASK));
	}
}
