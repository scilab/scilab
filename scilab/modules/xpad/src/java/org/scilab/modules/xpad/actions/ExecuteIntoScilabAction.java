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

public class ExecuteIntoScilabAction extends DefaultAction {

	private ExecuteIntoScilabAction(Xpad editor) {
		super(XpadMessages.EXECUTE_INTO_SCILAB, editor);
	}

	public void doAction() {
		/* Will execute the document file (file sould be saved)*/
		if (getEditor().save(getEditor().getTextPane())) {
			String filePath = getEditor().getFileFullPath();
			String cmdToExec = "exec('" + filePath + "', -1)";
			ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmdToExec, true, false);
		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu("Execute Into Scilab", null, new ExecuteIntoScilabAction(editor), null);
	 }
}
