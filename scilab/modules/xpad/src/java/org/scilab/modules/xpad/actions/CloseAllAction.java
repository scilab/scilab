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


import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

public class CloseAllAction extends DefaultAction {
    
    /**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = 7134703185408271944L;

	private CloseAllAction(Xpad editor) {
        super(XpadMessages.CLOSEALL, editor);
    }
    
    public void doAction() {
    	while (getEditor().getTabPane().getTabCount() != 0) {
    		getEditor().closeTabAt(getEditor().getTabPane().getSelectedIndex());
    	}
    	
    	// Close the last opened file create a new file named "Untitled 1"
    	if (getEditor().getTabPane().getTabCount() == 0) {
    		getEditor().addEmptyTab();
    	}
    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.CLOSEALL, null, new CloseAllAction(editor),null);
    }
}

