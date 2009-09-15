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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;

public class NewAction extends DefaultAction {

    private NewAction(Xpad editor) {
	super("New...", editor);
    }

    public void doAction() {
	getEditor().addEmptyTab();
    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu("New...", null, new NewAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.CTRL_MASK));
    }
    
    public static PushButton createButton(Xpad editor) {
	return createButton("New...", "document-new.png", new NewAction(editor));
    }
}
