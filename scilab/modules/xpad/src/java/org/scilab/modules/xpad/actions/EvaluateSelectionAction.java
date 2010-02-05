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

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * EvaluateSelectionAction class
 * @author Bruno JOFRET
 *
 */
public final class EvaluateSelectionAction extends DefaultAction {
	
	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 320938663765236236L;

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private EvaluateSelectionAction(Xpad editor) {
		super(XpadMessages.EVALUATE_SELECTION, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		/* Will do the job as if it was copy / paste in scilab Console */
		//InterpreterManagement.requestScilabExec(getEditor().getTextPane().getText());
		String selection = getEditor().getTextPane().getSelectedText();
		if (selection.compareTo("") != 0) {
			ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(selection, true, false);	
		}
	}

	/**
	 * createMenu
	 * @param editor xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.EVALUATE_SELECTION, null, new EvaluateSelectionAction(editor), null);
	}
}
