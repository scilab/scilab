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

package org.scilab.modules.xcos.block.actions;

import java.awt.Color;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;

import com.mxgraph.util.mxUtils;

public class ColorAction extends DefaultAction {

    private static final long serialVersionUID = -1253470374053230723L;
    private String key = null;
    private String title = null;
    
    private ColorAction(ScilabGraph scilabGraph, String title, String key) {
	super(title, scilabGraph);
	this.key = key;
	this.title = title;
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph, String title, String key) {
	return createMenu(title, null, new ColorAction(scilabGraph, title, key), null);
    }

    public void doAction() {
	XcosDiagram graph = (XcosDiagram) getGraph(null);
	Object[] selectedCells = graph.getSelectionCells();
	
	//if no cells are selected : Do nothing
	if (selectedCells.length == 0) { return; }

	Color newColor = JColorChooser.showDialog(getGraph(null).getAsComponent(), title, null);

	if (newColor != null)
	{
	    graph.setCellStyles(key, mxUtils.hexString(newColor));
    	}
    }
}
