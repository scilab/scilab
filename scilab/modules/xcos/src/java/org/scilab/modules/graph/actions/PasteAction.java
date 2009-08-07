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

public class PasteAction extends DefaultAction {
    
    public static PushButton pasteButton(ScilabGraph scilabGraph) {
	return createButton("Paste", "edit-paste.png", new PasteAction(scilabGraph));
    }
    
    public static MenuItem pasteMenu(ScilabGraph scilabGraph) {
	return createMenu("Paste", "edit-paste.png", new PasteAction(scilabGraph));
    }
    
    public PasteAction() {
	super();
    }
    
    public PasteAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
 
    public void actionPerformed(ActionEvent e) {
	TransferHandler.getPasteAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
		e.getID(), e.getActionCommand()));
    }
}
