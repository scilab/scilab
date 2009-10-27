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


import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.Toolkit;

import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class ExitAction extends DefaultAction {

    private ExitAction(Xpad editor) {
	super(XpadMessages.EXIT, editor);
    }

    public void doAction() {
    	ScilabWindow xpadWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getEditor().getParentWindowId());

    	int numberOfTab = getEditor().getTabPane().getComponentCount();

    	boolean wantToClose = true;
    	for ( int i = 0 ; i < numberOfTab ; i++){
    		//close and save all editors if they are modified
    		wantToClose &=  getEditor().closeTabAt(0);
    	}

    	if(wantToClose == true){
    		xpadWindow.getAsSimpleWindow().removeTab(getEditor());
    		Xpad.closeXpad();
    	}
    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.EXIT, null, new ExitAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_Q, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
}