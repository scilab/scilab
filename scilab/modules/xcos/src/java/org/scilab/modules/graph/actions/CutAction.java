/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;
import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

public class CutAction extends DefaultAction {
    
    public CutAction(ScilabGraph scilabGraph) {
    	super(ScilabGraphMessages.CUT, scilabGraph);
    }
 
    public static PushButton cutButton(ScilabGraph scilabGraph) {
    	return createButton(ScilabGraphMessages.CUT, "edit-cut.png", new CutAction(scilabGraph));
    }
    
    public static MenuItem cutMenu(ScilabGraph scilabGraph) {
    	return createMenu(ScilabGraphMessages.CUT, "edit-cut.png", new CutAction(scilabGraph), KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.CTRL_MASK));
    }
    
    public void actionPerformed(ActionEvent e) {
    	TransferHandler.getCutAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(), e.getID(), e.getActionCommand()));
    }
}
