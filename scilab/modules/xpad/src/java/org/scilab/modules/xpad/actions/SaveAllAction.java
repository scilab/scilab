/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * 
 * @author Allan CORNET
 * SaveAllAction class
 */
public final class SaveAllAction extends DefaultAction {


	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 7134703185408281234L;

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private SaveAllAction(Xpad editor) {
			super(XpadMessages.SAVE_ALL, editor);
		    }

	/**
	 * doAction
	 */
	public void doAction() {
		int currentIndex = getEditor().getTabPane().getSelectedIndex();
		
		for (int i = 0; i < getEditor().getTabPane().getTabCount(); i++) {
			getEditor().getTabPane().setSelectedIndex(i);
			if (!getEditor().save(getEditor().getTabPane().getSelectedIndex(), true)) {
					ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.COULD_NOT_SAVE_FILE,
						XpadMessages.XPAD_ERROR, IconType.ERROR_ICON);
				    }
		}
		getEditor().getTabPane().setSelectedIndex(currentIndex);
			
	}
	
	/**
	 * CreateMenu
	 * @param editor Xpad
	 * @return MenuItem 
	 */
    public static MenuItem createMenu(Xpad editor) {
    	return createMenu(XpadMessages.SAVE_ALL, null, new SaveAllAction(editor), 
    			KeyStroke.getKeyStroke(KeyEvent.VK_S, 
    			Toolkit.getDefaultToolkit().getMenuShortcutKeyMask() + ActionEvent.SHIFT_MASK));
        }
}
