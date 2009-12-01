/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET 
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
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

@SuppressWarnings("serial")
public class LoadIntoScilabAction extends DefaultAction {
	
	public static final String TMP_FILENAME = "LINES_LOAD_INTO_SCILAB.sce"; 
	
	private LoadIntoScilabAction(Xpad editor) {
		super(XpadMessages.LOAD_INTO_SCILAB, editor);
	}

	public void doAction() {
		boolean bDoExec = false;
		String TMP_FULLFILENAME = System.getenv("TMPDIR") + System.getProperty("file.separator") + TMP_FILENAME;
		// save file as UTF-8
		File f = new File(TMP_FULLFILENAME);
		try {
			OutputStreamWriter out = new OutputStreamWriter(new FileOutputStream(f) , "UTF-8");
			out.write(getEditor().getTextPane().getText());
			out.flush();
			out.close();
			bDoExec = true;
		}
		catch(Exception e) {
			bDoExec = false;
		}

		if (bDoExec && f.exists()) {
			String cmdToExec = "exec('" + TMP_FULLFILENAME + "')";
			InterpreterManagement.requestScilabExec(cmdToExec);
		} else {
			ScilabModalDialog.show(getEditor(), XpadMessages.COULD_NOT_FIND_TMPFILE);	
		}
	}
	
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.LOAD_INTO_SCILAB, null, new LoadIntoScilabAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_L, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
