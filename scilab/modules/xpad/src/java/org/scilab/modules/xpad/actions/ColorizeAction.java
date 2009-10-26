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

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Colorization management
 * @author Bruno JOFRET
 */
public final class ColorizeAction extends DefaultAction {

	private static final long serialVersionUID = -2486375196709197718L;

	private static Xpad colorEditor;

	/**
	 * Constructor
	 * @param editor associated Xpad instance
	 */
	private ColorizeAction(Xpad editor) {
		super(XpadMessages.COLORIZE, editor);
		colorEditor = editor;
	}

	/**
	 * Colorize action
	 * @see org.scilab.modules.xpad.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument())
			.colorize(0, getEditor().getTextPane().getDocument().getLength());
	}

	/**
	 * Create a menu to add to Xpad menu bar
	 * @param editor associated Xpad instance
	 * @return the menu
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.COLORIZE, null, new ColorizeAction(editor), null);
	}

	/**
	 * Get current instance
	 */
	public static void getXpadEditor() {
		((ScilabStyleDocument) colorEditor.getTextPane().getStyledDocument()).setEditor(colorEditor);
	}
}
