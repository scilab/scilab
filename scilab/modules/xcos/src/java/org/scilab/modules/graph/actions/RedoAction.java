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

import org.scilab.modules.graph.ScilabGraph;

public class RedoAction extends DefaultAction {
    
    public static JButton redoButton(ScilabGraph scilabGraph) {
	return createButton("Redo", "/org/scilab/modules/graph/resources/redo.png", new RedoAction(scilabGraph));
    }
    
    public static JMenuItem redoMenu(ScilabGraph scilabGraph) {
	return createMenu("Redo", "/org/scilab/modules/graph/resources/redo.png", new RedoAction(scilabGraph));
    }
    
    public RedoAction() {
	super();
    }
    
    public RedoAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
    
    public void actionPerformed(ActionEvent e) {
	getGraph(e).redo();
    }

  

}
