/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO -  Allan CORNET
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

/**
 * CloseAllButThisAction Class
 * @author Allan CORNET
 *
 */
public final class  CloseAllButThisAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -9052217229357472945L;

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private CloseAllButThisAction(Xpad editor) {
	        super(XpadMessages.CLOSEALLBUTTHIS, editor);
	}
	
	/**
	 * doAction
	 */
	public void doAction() {
		int nbTabCount = getEditor().getTabPane().getTabCount();
		boolean bContinue = true;
		if (nbTabCount > 1) {
			while ((getEditor().getTabPane().getTabCount() != 1) & (bContinue)) {
				int currentIndex = getEditor().getTabPane().getSelectedIndex();
				if (currentIndex != 0) {
					bContinue = getEditor().closeTabAt(0);
				} else {
					bContinue = getEditor().closeTabAt(1);
				}
			}
		}
		
	   	// Close the last opened file create a new file named "Untitled 1"
	   	if (getEditor().getTabPane().getTabCount() == 0) {
	   		getEditor().addEmptyTab();
	   	}
	}
	    
	/**
	 * createMenu
	 * @param editor Xpad
	 * @return MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.CLOSEALLBUTTHIS, null, new CloseAllButThisAction(editor), null);
	}
}
