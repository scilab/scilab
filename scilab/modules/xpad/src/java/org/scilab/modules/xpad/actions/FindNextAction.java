/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS 
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
import java.util.ArrayList;

import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.style.SearchManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * 
 * @author Antoine ELIAS
 *
 */
public final class FindNextAction extends DefaultAction {

	private SearchManager searchManager = new SearchManager();

	private static final long serialVersionUID = -9017015781643180532L;

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	protected FindNextAction(Xpad editor) {
		super(XpadMessages.FIND_NEXT, editor);
	}

	/**
	 * DoAction
	 */
	public void doAction() {
		try {
			int startPos = getEditor().getTextPane().getSelectionStart();
			int endPos = getEditor().getTextPane().getSelectionEnd();
			int startLine = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(startPos);
			int endLine = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getDefaultRootElement().getElementIndex(endPos);

			//don't manage multiple lines quick find 
			if(startLine != endLine) {
				return;
			}

			String exp = null;
			if(startPos == endPos) {
				//nothing to search
				if(FindAction.getPreviousSearch() == null) {
					return;
				} else {
					exp = FindAction.getPreviousSearch();
				}
			} else {
				exp = getEditor().getTextPane().getDocument().getText(startPos, endPos - startPos);
			}
	
			JTextPane xpadTextPane =  getEditor().getTextPane();
			ScilabStyleDocument scilabStyle = ((ScilabStyleDocument) xpadTextPane.getStyledDocument());
			
			//search from current position to end document
			ArrayList<Integer[]> offsets = searchManager.findWord(scilabStyle, exp, 0, scilabStyle.getLength(), false, false, false);
			if(offsets.size() != 0) {
				int index = -1;
				for (int i = 0; i < offsets.size(); i++) {
					if (offsets.get(i)[0] >= endPos + 1) {
						index = i;
						break;
					}
				}
				
				if(index == -1) {
					index = 0;
				}
				
				getEditor().getTextPane().select(offsets.get(index)[0], offsets.get(index)[1]);
			}
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.FIND_NEXT, null, new FindNextAction(editor),
				KeyStroke.getKeyStroke(KeyEvent.VK_K, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

}
