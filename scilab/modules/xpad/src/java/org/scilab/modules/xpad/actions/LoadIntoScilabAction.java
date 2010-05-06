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
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
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
			boolean bContinue = false;

			if (maxLinesRecommandedDetected(text)) {
				if (ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.BIG_FILE_WARNING, XpadMessages.LOAD_INTO_SCILAB, 
				 IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION) {
					bContinue = true;
				} else {
					bContinue = false;
				}
			} else {
				bContinue = true;
			}

			if (bContinue) {
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
					String cmdToExec = "exec('" + tmpFullFilename + "', 1)";
					InterpreterManagement.requestScilabExec(cmdToExec);
				} else {
					ScilabModalDialog.show(getEditor(), XpadMessages.COULD_NOT_FIND_TMPFILE);
				}
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

	 /**
	  * check if string is not too big
	  * @param[in] text to scan
	  * @return true or false
	  */
	private static boolean maxLinesRecommandedDetected(String text) {
		int MAX_LINES_RECOMMANDED = 100;
		int NB_STANDARD_COLUMNS = 80;
		int MAX_CHARACTERS_RECOMMANDED = MAX_LINES_RECOMMANDED * NB_STANDARD_COLUMNS;
		if (text.length() > MAX_CHARACTERS_RECOMMANDED) {
			return true;
		}

		String splitedStringOnLineSeparator[] = text.split(System.getProperty("line.separator"));
		if (splitedStringOnLineSeparator.length > MAX_LINES_RECOMMANDED) {
			return true;
		}

		return false;
	}
}
