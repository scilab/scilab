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
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JMenuItem;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.swing.util.mxGraphActions;

public class ZoomInAction extends DefaultAction implements ActionListener {
    
    public static JButton zoominButton(ScilabGraph scilabGraph) {
	return createButton("Zoom In", "/org/scilab/modules/graph/resources/zoomin.gif", new ZoomInAction(scilabGraph));
    }
    
    public static JMenuItem zoominMenu(ScilabGraph scilabGraph) {
	return createMenu("Zoom In", "/org/scilab/modules/graph/resources/zoomin.gif", new ZoomInAction(scilabGraph));
    }
    
    public ZoomInAction() {
	super();
    }
    
    public ZoomInAction(ScilabGraph scilabGraph) {
	super(scilabGraph);
    }
    
    public void actionPerformed(ActionEvent e) {
	mxGraphActions.getZoomInAction().actionPerformed(new ActionEvent(getGraph(e).getAsComponent(),
		e.getID(), e.getActionCommand()));
    }

}
