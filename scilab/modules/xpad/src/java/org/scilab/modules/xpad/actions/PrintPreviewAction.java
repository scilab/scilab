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
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.PrinterWriter;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.XpadPrintPreviewWindow;
import org.scilab.modules.xpad.utils.XpadMessages;

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
	 * @param editor Xpad
	 */
	private PrintPreviewAction(Xpad editor) {
		super(XpadMessages.PRINT_PREVIEW, editor);
	}

	/**
	 * doAction
	 */
	public void doAction() {
		PrinterWriter printerWriter = new PrinterWriter(getEditor().getTextPane());
		new XpadPrintPreviewWindow(printerWriter, getEditor());
	}

	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.PRINT_PREVIEW, null, new PrintPreviewAction(editor), 
				KeyStroke.getKeyStroke(KeyEvent.VK_P, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask() + ActionEvent.SHIFT_MASK));
	}

	/**
	 * createButton
	 * @param editor Xpad
	 * @return PushButton
	 */
	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.PRINT_PREVIEW, "document-print-preview.png", new PrintPreviewAction(editor));
	}

}
