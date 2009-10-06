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
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * File creation management
 * @author Bruno JOFRET
 */
public final class NewAction extends DefaultAction {

	private static final long serialVersionUID = -6865132453503118587L;

	/**
	 * Constructor
	 * @param editor associated Xpad instance
	 */
	private NewAction(Xpad editor) {
		super(XpadMessages.NEW, editor);
	}

	/**
	 * Create file action
	 * @see org.scilab.modules.xpad.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		getEditor().addEmptyTab();
	}

	/**
	 * Create a menu to add to Xpad menu bar
	 * @param editor associated Xpad instance
	 * @return the menu
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.NEW, null, new NewAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_N, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Create a button to add to Xpad tool bar
	 * @param editor associated Xpad instance
	 * @return the button
	 */
	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.NEW, "document-new.png", new NewAction(editor));
	}
}
