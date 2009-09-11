/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
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

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;

public class HighlightCurrentLineAction extends DefaultCheckAction {

    private HighlightCurrentLineAction(Xpad editor) {
	super("Highlight current line", editor);
	setState(false);
    }

    public void doAction() {
	getEditor().enableLineHighlight(this.getState());
    }

    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor) {
	return createCheckBoxMenu("Highlight current line", null, new HighlightCurrentLineAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_J, ActionEvent.CTRL_MASK));
    }
}
