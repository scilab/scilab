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

import java.awt.print.PageFormat;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.PrinterWriter;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class Print action for SciNotes
 * @author Sylvestre Koumar
 *
 */
public class PrintAction extends DefaultAction {

	/**
	 * Default constructor
	 * @param editor the editor
	 */
	private PrintAction(SciNotes editor) {
		super(SciNotesMessages.PRINT, editor);
	}

	/**
	 * Function doAction
	 */
	public void doAction() {
		printSciNotesDocument(getEditor());
	}

	/**
	 * Create the MenuItem for print action
	 * @param editor Editor
	 * @param key KeyStroke
	 * @return a MenuItem
	 */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	    return createMenu(SciNotesMessages.PRINT, null, new PrintAction(editor), key);
	}

	/**
	 * Create print button
	 * @param editor Editor
	 * @return a pushbutton
	 */
	public static PushButton createButton(SciNotes editor) {
		return createButton(SciNotesMessages.PRINT, "document-print.png", new PrintAction(editor));
	}

	/**
	 * This function allow to print a document
	 * by calling a printer job
	 * @param editor Editor
	 * @return a boolean
	 */
	public static boolean printSciNotesDocument(SciNotes editor) {

		PrinterJob printTask = PrinterJob.getPrinterJob();

		PageFormat pageFormat = PageSetupAction.getPageFormat();
		if (pageFormat != null) {
			printTask.setPrintable(new PrinterWriter(editor.getTextPane()), pageFormat);
		} else {
			printTask.setPrintable(new PrinterWriter(editor.getTextPane()));
		}

		if (printTask.printDialog()) { 
			try {
				printTask.print();
			} catch (PrinterException e) {
				e.printStackTrace();
				return false;
			}
		}
		return true;
	}
}
