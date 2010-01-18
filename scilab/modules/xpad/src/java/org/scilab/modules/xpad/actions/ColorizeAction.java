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

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ColorizationManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;
/**
 * Colorization management
 * @author Bruno JOFRET
 */
public final class ColorizeAction extends DefaultCheckAction {

	private static final long serialVersionUID = -2486375196709197718L;


	private ColorizationManager colorizationManager = new ColorizationManager();

	/**
	 * Constructor
	 * @param editor associated Xpad instance
	 */
	private ColorizeAction(Xpad editor) {
		super(XpadMessages.COLORIZE, editor);
	}

	/**
	 * Colorize action
	 * @see org.scilab.modules.xpad.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		ScilabStyleDocument scilabDocument = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument(); 

		if (!this.getState()) {
			colorizationManager.resetStyle(scilabDocument);
		} else {
			colorizationManager.colorize(scilabDocument, 0, scilabDocument.getLength());
		}
		getEditor().setAutoColorize(this.getState());
		ConfigXpadManager.saveAutoColorize(this.getState());
	}

	/**
	 * Create a menu to add to Xpad menu bar
	 * @param editor associated Xpad instance
	 * @return the menu
	 */
	public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor) {
		CheckBoxMenuItem colorize = createCheckBoxMenu(XpadMessages.COLORIZE, null, new ColorizeAction(editor), null);
		colorize.setChecked(ConfigXpadManager.getAutoColorize());
    	return colorize;
	}
}
