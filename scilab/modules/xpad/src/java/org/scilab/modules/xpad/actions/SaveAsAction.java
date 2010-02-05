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

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * 
 * @author Bruno JOFRET
 *
 */
public final class SaveAsAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 8327808176820789884L;


	/**
	 * Constructor
	 * @param editor Xpad
	 */
    private SaveAsAction(Xpad editor) {
	super(XpadMessages.SAVE_AS, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
    	if (!getEditor().saveAs(getEditor().getTextPane())) {
    		ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.COULD_NOT_SAVE_FILE,
    				XpadMessages.XPAD_ERROR, IconType.ERROR_ICON);
    	}
    }

    /**
     * CreateMenu
     * @param editor Xpad
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor) {
    	return createMenu(XpadMessages.SAVE_AS, null, new SaveAsAction(editor),
    			KeyStroke.getKeyStroke(KeyEvent.VK_S, 
    			Toolkit.getDefaultToolkit().getMenuShortcutKeyMask() + ActionEvent.ALT_MASK));
    }

    
    /**
     * Create Button
     * @param editor Xpad
     * @return PushButton
     */
    public static PushButton createButton(Xpad editor) {
    	return createButton(XpadMessages.SAVE_AS, "document-save-as.png", new SaveAsAction(editor));
    }
}
