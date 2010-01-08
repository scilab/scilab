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
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.UUID;

import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * LoadIntoScilabAction class
 * @author Allan CORNET
 *
 */
public final class LoadIntoScilabAction extends DefaultAction {
	
	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -5659317486404897280L;
	

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private LoadIntoScilabAction(Xpad editor) {
		super(XpadMessages.LOAD_INTO_SCILAB, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		boolean bDoExec = false;
		String text = getEditor().getTextPane().getText();
		
		if (text.compareTo("") != 0) {
			String tmpFilename = "LOAD_INTO_SCILAB-" + UUID.randomUUID().toString() + ".sce";
			String tmpFullFilename = System.getenv("TMPDIR") + System.getProperty("file.separator") + tmpFilename;
			// save file as UTF-8
			File f = new File(tmpFullFilename);
			try {
				OutputStreamWriter out = new OutputStreamWriter(new FileOutputStream(f) , "UTF-8");
				out.write(text);
				out.flush();
				out.close();
				bDoExec = true;
			} catch (IOException e) {
				bDoExec = false;
			}
			
			if (bDoExec && f.exists()) {
				String cmdToExec = "exec('" + tmpFullFilename + "', 2)";
				InterpreterManagement.requestScilabExec(cmdToExec);
			} else {
				ScilabModalDialog.show(getEditor(), XpadMessages.COULD_NOT_FIND_TMPFILE);	
			}
		}
	}
	
	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.LOAD_INTO_SCILAB, null, 
				new LoadIntoScilabAction(editor), 
				KeyStroke.getKeyStroke(KeyEvent.VK_L, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	 }
}
