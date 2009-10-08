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

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class LineNumbersAction extends DefaultCheckAction {

    private LineNumbersAction(Xpad editor) {
	super(XpadMessages.LINE_NUMBERS, editor);
	setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_L, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }

    public void doAction() {
	getEditor().displayLineNumbers(this.getState());
    }

    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor) {
    CheckBoxMenuItem lineNumber = createCheckBoxMenu(XpadMessages.LINE_NUMBERS, null, new LineNumbersAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_L, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    lineNumber.setChecked(true);
	return lineNumber;
    }
}
