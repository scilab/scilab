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
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * AutoIndentAction Class
 * @author Bruno JOFRET
 *
 */
public final class AutoIndentAction extends DefaultCheckAction  {


	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -1937347660350539353L;


	/**
	 * Constructor
	 * @param editor Xpad
	 */
    private AutoIndentAction(Xpad editor) {
    	super(XpadMessages.AUTO_INDENT, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
    	getEditor().setAutoIndent(this.getState());
    	ConfigXpadManager.saveAutoIndent(this.getState());
    }


    /**
     * createCheckBoxMenu
     * @param editor Xpad
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor) {
    	CheckBoxMenuItem autoIndent = createCheckBoxMenu(XpadMessages.AUTO_INDENT, null, new AutoIndentAction(editor), null);
    	autoIndent.setChecked(ConfigXpadManager.getAutoIndent());
    	return autoIndent;
    }
}
