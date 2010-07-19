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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.PrinterWriter;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesPrintPreviewWindow;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * PrintPreviewAction class
 * @author Bruno JOFRET
 *
 */
public final class PrintPreviewAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -3222532237364937814L;

	/**
	 * Constructor
	 * @param editor SciNotes
	 */
	private PrintPreviewAction(SciNotes editor) {
		super(SciNotesMessages.PRINT_PREVIEW, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		PrinterWriter printerWriter = new PrinterWriter(getEditor().getTextPane());
		new SciNotesPrintPreviewWindow(printerWriter, getEditor());
	}

	/**
	 * createMenu
	 * @param editor SciNotes
	 * @param key KeyStroke
	 * @return MenuItem
	 */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
	    return createMenu(SciNotesMessages.PRINT_PREVIEW, null, new PrintPreviewAction(editor), key);
	}

	/**
	 * createButton
	 * @param editor SciNotes
	 * @return PushButton
	 */
	public static PushButton createButton(SciNotes editor) {
		return createButton(SciNotesMessages.PRINT_PREVIEW, "document-print-preview.png", new PrintPreviewAction(editor));
	}

}
