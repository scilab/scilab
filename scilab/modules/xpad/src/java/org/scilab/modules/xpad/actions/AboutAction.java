/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
import org.scilab.modules.gui.utils.ScilabAboutBox;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Display about box for the text editor
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class AboutAction extends DefaultAction {

	private static final long serialVersionUID = -8172282717877209957L;

	/**
	 * Constructor
	 * @param editor associated editor
	 */
	private AboutAction(Xpad editor) {
		super(XpadMessages.ABOUT, editor);
	}

	/**
	 * Menu created for "Help menu"
	 * @param editor associated editor
	 * @return the menu
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.ABOUT, null, new AboutAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_F1, ActionEvent.SHIFT_MASK));
	}

	/**
	 * Action !!
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		String[] contents = {XpadMessages.VERSION,
				"",
				XpadMessages.DIGITEO,
				XpadMessages.COPYRIGHT};

		ScilabAboutBox.createAboutBox(XpadMessages.ABOUT, contents, null, null);
	}
}
