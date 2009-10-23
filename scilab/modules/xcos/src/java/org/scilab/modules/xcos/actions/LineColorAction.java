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

package org.scilab.modules.xcos.actions;

import java.awt.Color;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

public class LineColorAction extends DefaultAction {

    private LineColorAction(ScilabGraph scilabGraph) {
	super(XcosMessages.LINE_COLOR, scilabGraph);
    }

    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(XcosMessages.LINE_COLOR, null, new LineColorAction(scilabGraph), null);
    }

    public void doAction() {
	XcosDiagram graph = (XcosDiagram) getGraph(null);
	Object[] selectedCells = graph.getSelectionCells();
	List<BasicLink> links = new ArrayList<BasicLink>();
	
	//if no cells are selected : Do nothing
	if (selectedCells.length == 0) { return; }

	Color newColor = JColorChooser.showDialog(getGraph(null).getAsComponent(), XcosMessages.LINE_COLOR, null);

	if (newColor != null)
	{
	    graph.setCellStyles(mxConstants.STYLE_STROKECOLOR, mxUtils.hexString(newColor));
    	}
    }
}
