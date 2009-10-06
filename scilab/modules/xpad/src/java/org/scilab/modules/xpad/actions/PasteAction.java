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
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class PasteAction extends DefaultAction {

    private PasteAction(Xpad editor) {
	super(XpadMessages.PASTE, editor);
    }
    
    public void doAction() {
	getEditor().getTextPane().getActionMap().get(DefaultEditorKit.pasteAction).actionPerformed(null);
    }

    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.PASTE, null, new PasteAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_V, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
    
    public static PushButton createButton(Xpad editor) {
	return createButton(XpadMessages.PASTE, "edit-paste.png", new PasteAction(editor));
    }
}
