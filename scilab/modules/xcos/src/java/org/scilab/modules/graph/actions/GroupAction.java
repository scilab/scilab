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

import com.mxgraph.swing.util.mxGraphActions;

public class GroupAction extends DefaultAction {
    
    public static JButton groupButton(ScilabGraph scilabGraph) {
	return createButton("Group", "/org/scilab/modules/graph/resources/group.gif", new GroupAction(scilabGraph));
    }
    
    public static JMenuItem groupMenu(ScilabGraph scilabGraph) {
	return createMenu("Group", "/org/scilab/modules/graph/resources/group.gif", new GroupAction(scilabGraph));
    }
    
    public GroupAction() {
	super();
    }
    
    public GroupAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
    
    public void actionPerformed(ActionEvent e) {
	mxGraphActions.getGroupAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
		e.getID(), e.getActionCommand()));
    }

}
