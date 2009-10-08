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
import java.io.File;
import java.io.FileWriter;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class SaveAction extends DefaultAction {

	private SaveAction(Xpad editor) {
		super(XpadMessages.SAVE, editor);

	}

	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.SAVE, null, new SaveAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_S, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.SAVE, "media-floppy.png", new SaveAction(editor));
	}

	public void doAction() {
		getEditor().save(getEditor().getTextPane());

	}
}

