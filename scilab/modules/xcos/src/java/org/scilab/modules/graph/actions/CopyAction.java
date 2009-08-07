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

package org.scilab.modules.graph.actions;

import java.awt.event.ActionEvent;

import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;

public class CopyAction extends DefaultAction {

    public static PushButton copyButton(ScilabGraph scilabGraph) {
	return createButton("Copy", "edit-copy.png", new CopyAction(scilabGraph));
    }
    
    public static MenuItem copyMenu(ScilabGraph scilabGraph) {
	return createMenu("Copy", "edit-copy.png", new CopyAction(scilabGraph));
    }
    
    public CopyAction() {
	super();
    }
    
    public CopyAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
 
    public void actionPerformed(ActionEvent e) {
	TransferHandler.getCopyAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
		e.getID(), e.getActionCommand()));
    }
}
