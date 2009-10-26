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

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.print.PageFormat;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;
import java.awt.Toolkit;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.PrinterWriter;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class PrintAction extends DefaultAction {

	private PrintAction(Xpad editor) {
		super(XpadMessages.PRINT, editor);
	}

	public void doAction() {
		printXpadDocument(getEditor());
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.PRINT, null, new PrintAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_P, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.PRINT, "document-print.png", new PrintAction(editor));
	}

	public static boolean printXpadDocument(Xpad editor) {

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
