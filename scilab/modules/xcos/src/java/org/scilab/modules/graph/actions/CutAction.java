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

import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.TransferHandler;

import org.scilab.modules.graph.ScilabGraph;

public class CutAction extends DefaultAction {
    
    public static JButton cutButton(ScilabGraph scilabGraph) {
	return createButton("Cut", "/org/scilab/modules/graph/resources/edit-cut.png", new CutAction(scilabGraph));
    }
    
    public static JMenuItem cutMenu(ScilabGraph scilabGraph) {
	return createMenu("Cut", "/org/scilab/modules/graph/resources/edit-cut.png", new CutAction(scilabGraph));
    }
    
    public CutAction() {
	super();
    }
    
    public CutAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
 
    public void actionPerformed(ActionEvent e) {
	TransferHandler.getCutAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
		e.getID(), e.getActionCommand()));
    }
}
