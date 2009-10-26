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

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class HelpAction extends DefaultAction {

	private HelpAction(Xpad editor) {
		super(XpadMessages.HELP, editor);
	}

	public void doAction() {
		InterpreterManagement.requestScilabExec("help('xpad')");
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.HELP, null, new HelpAction(editor), null);
	}
}
