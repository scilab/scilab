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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * File creation management
 * @author Bruno JOFRET
 */
public final class NewAction extends DefaultAction {

	private static final long serialVersionUID = -6865132453503118587L;

	/**
	 * Constructor
	 * @param editor associated SciNotes instance
	 */
	private NewAction(SciNotes editor) {
		super(SciNotesMessages.NEW, editor);
	}

	/**
	 * Create file action
	 * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		getEditor().addEmptyTab();
	}

	/**
	 * Create a menu to add to SciNotes menu bar
	 * @param editor associated SciNotes instance
	 * @param key KeyStroke
	 * @return the menu
	 */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	    return createMenu(SciNotesMessages.NEW, null, new NewAction(editor), key); 
	}

	/**
	 * Create a button to add to SciNotes tool bar
	 * @param editor associated SciNotes instance
	 * @return the button
	 */
	public static PushButton createButton(SciNotes editor) {
		return createButton(SciNotesMessages.NEW, "document-new.png", new NewAction(editor));
	}
}
