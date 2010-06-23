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

package org.scilab.modules.scinotes.actions;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class Previous tab action for SciNotes
 * @author Sylvestre Koumar
 *
 */
public class PreviousTabAction extends DefaultAction {

	/**
	 * Default constructor
	 * @param editor Editor
	 */
	private PreviousTabAction(SciNotes editor) {
		super(SciNotesMessages.PREVIOUS_TAB, editor);
	}
	
	/**
	 * Function Run
	 */
	public void doAction() {
		this.getEditor().getTabPane().getSelectedIndex();
	}

	/**
	 * Create the MenuItem for previous tab action
	 * @param editor Editor
	 * @return a MenuItem
	 */
	public static MenuItem createMenu(SciNotes editor) {
		return createMenu(SciNotesMessages.PREVIOUS_TAB, null, new PreviousTabAction(editor), 
						  KeyStroke.getKeyStroke(KeyEvent.VK_TAB, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()
								  				 + ActionEvent.SHIFT_MASK));
	 }
}
