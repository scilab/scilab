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

import javax.swing.JTextPane;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class ExecuteFileIntoScilabAction extends DefaultAction {
	
	private static int CANCEL = 1;
	private static int SAVE_AND_EXECUTE = 2;

	private ExecuteFileIntoScilabAction(Xpad editor) {
		super(XpadMessages.EXECUTE_FILE_INTO_SCILAB, editor);
	}

	public void doAction() {
	    /* Will execute the document file (file sould be saved)*/

	    String filePath = "";
	    Xpad editor = getEditor();
	    JTextPane currentPane = getEditor().getTextPane();

	    if (((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).isContentModified()) {
		if (ScilabModalDialog.show(XpadMessages.EXECUTE_WARNING, XpadMessages.EXECUTE_FILE_INTO_SCILAB, 
			IconType.WARNING_ICON, ButtonType.CANCEL_OR_SAVE_AND_EXECUTE) == AnswerOption.SAVE_EXECUTE_OPTION) {
		    if (editor.save(getEditor().getTabPane().getSelectedIndex(), false)) {
			filePath = editor.getFileFullPath();
			String cmdToExec = "exec('" + filePath + "', -1)";
			ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmdToExec, true, false);
		    }
		} 
	    } else {
		filePath = editor.getFileFullPath();
		String cmdToExec = "exec('" + filePath + "', -1)";
		ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmdToExec, true, false);
	    }
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.EXECUTE_FILE_INTO_SCILAB, null, new ExecuteFileIntoScilabAction(editor), null);
	 }
}
