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
import java.awt.Toolkit;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.PrinterWriter;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.XpadPrintPreviewWindow;
import org.scilab.modules.xpad.utils.XpadMessages;

public class PrintPreviewAction extends DefaultAction {

	private PrintPreviewAction(Xpad editor) {
		super(XpadMessages.PRINT_PREVIEW, editor);
	}

	public void doAction() {
		new XpadPrintPreviewWindow(new PrinterWriter(getEditor().getTextPane()), getEditor());
	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.PRINT_PREVIEW, null, new PrintPreviewAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_P, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()+ActionEvent.SHIFT_MASK));
	}

	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.PRINT_PREVIEW, "document-print-preview.png", new PrintPreviewAction(editor));
	}

}
