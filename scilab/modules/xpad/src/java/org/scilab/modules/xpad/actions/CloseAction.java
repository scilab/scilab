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

public class CloseAction extends DefaultAction {
    
    private CloseAction(Xpad editor) {
        super(XpadMessages.CLOSE, editor);
    }
    
    public void doAction() {
    	getEditor().closeCurrentTab();
    	
    	// Close the last opened file create a new file named "Untitled 1"
    	if (getEditor().getTabPane().getTabCount() == 0) {
    		getEditor().addEmptyTab();
    	}
    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.CLOSE, null, new CloseAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_W, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
}