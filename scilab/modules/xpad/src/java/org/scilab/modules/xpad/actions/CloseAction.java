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
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * CloseAction Class
 * @author Bruno JOFRET
 *
 */
public final class CloseAction extends DefaultAction {
    
    /**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 3575152401442746355L;

	/**
	 * Constructor 
	 * @param editor Xpad
	 */
	private CloseAction(Xpad editor) {
        super(XpadMessages.CLOSE, editor);
    }
    
	/**
	 * DoAction
	 */
    public void doAction() {
    	getEditor().closeTabAt(getEditor().getTabPane().getSelectedIndex());
    	
    	// Close the last opened file create a new file named "Untitled 1"
    	if (getEditor().getTabPane().getTabCount() == 0) {
    		getEditor().addEmptyTab();
    	}
    }
    
    /**
     * CreateMenu
     * @param editor Xpad
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.CLOSE, null, new CloseAction(editor), 
			KeyStroke.getKeyStroke(KeyEvent.VK_W, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
}

