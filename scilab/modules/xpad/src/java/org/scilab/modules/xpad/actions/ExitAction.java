/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * ExitAction class
 * @author Bruno JOFRET
 *
 */
public final class ExitAction extends DefaultAction {

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -6434487252794798547L;

	/**
	 * Constructor 
	 * @param editor Xpad
	 */
    private ExitAction(Xpad editor) {
	super(XpadMessages.EXIT, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
    	doExit(getEditor());
    }
    
    /**
     * doExit
     * @param editor Xpad
     */
    public static void doExit(Xpad editor) {
    	ScilabWindow xpadWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(editor.getParentWindowId());

    	int numberOfTab = editor.getTabPane().getComponentCount();

    	boolean wantToClose = true;
    	int k = 0;
    	for (int i = 0; i < numberOfTab; i++) {
    		//close and save all editors if they are modified
    		boolean response = editor.closeTabAt(k); 
    		if (!response) {
    			k++;
    		}
    		wantToClose &= response;  
    	}

    	if (wantToClose) {
    		xpadWindow.getAsSimpleWindow().removeTab(editor);
    		Xpad.closeXpad();
    	}
    }
    
    /**
     * createMenu
     * @param editor Xpad
     * @return MenuItem
     */
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.EXIT, null, new ExitAction(editor), 
			KeyStroke.getKeyStroke(KeyEvent.VK_Q, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
}
