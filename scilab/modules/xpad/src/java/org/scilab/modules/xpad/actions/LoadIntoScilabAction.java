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

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

@SuppressWarnings("serial")
public class LoadIntoScilabAction extends DefaultAction {
	
	private LoadIntoScilabAction(Xpad editor) {
		super(XpadMessages.LOAD_INTO_SCILAB, editor);
	}

	public void doAction() {
	    /* Will do the job as if it was copy / paste in scilab Console */
	    try {
	    	ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(getEditor().getTextPane().getText(), true, false);
	    } catch (NoClassDefFoundError noClass) {
			try {
				InterpreterManagement.requestScilabExec(getEditor().getTextPane().getText().replaceAll("\n", ","));
			} catch (NoClassDefFoundError noAlternateClass) {
				/* This happens when Xpad is launch as standalone (ie without
				* Scilab) */
				Xpad editor = getEditor();
				ScilabModalDialog.show(editor, "Could not find the console nor the InterpreterManagement");
			}
	    }
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.LOAD_INTO_SCILAB, null, new LoadIntoScilabAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_L, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
