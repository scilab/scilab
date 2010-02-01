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

import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * HelpAction Class
 * @author Bruno JOFRET
 *
 */
public final class HelpAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -3024180292998640447L;

	/**
	 * Constructor 
	 * @param editor Xpad
	 */
	private HelpAction(Xpad editor) {
		super(XpadMessages.HELP, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		String selection = getEditor().getTextPane().getSelectedText();
		if (selection == null || selection.equals("")) {
			InterpreterManagement.requestScilabExec("help('editor')");
		} else {
			InterpreterManagement.requestScilabExec("help('" + selection + "')");
		}
	}

	/**
	 * createMenu 
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.HELP, null, new HelpAction(editor), 
				KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0));
	}
}
