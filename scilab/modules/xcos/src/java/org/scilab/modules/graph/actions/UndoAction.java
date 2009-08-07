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

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;

import org.scilab.modules.graph.ScilabGraph;

public class UndoAction extends DefaultAction {

    public static JButton undoButton(ScilabGraph scilabGraph) {
	return createButton("Undo", "/org/scilab/modules/graph/resources/undo.png", new UndoAction(scilabGraph));
    }
    
    public static JMenuItem undoMenu(ScilabGraph scilabGraph) {
	return createMenu("Undo", "/org/scilab/modules/graph/resources/undo.png", new UndoAction(scilabGraph));
    }
    
    public UndoAction() {
	super();
    }
    
    public UndoAction(ScilabGraph editgraph) {
	super(editgraph);
    }

    public void actionPerformed(ActionEvent e) {
	getGraph(e).undo();
    }

}
