/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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
import java.awt.event.ActionEvent;

import javax.swing.JColorChooser;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;

/**
 * Change the filled color of multiple blocks
 */
public final class FilledColorAction extends VertexSelectionDependantAction {
	public static final String NAME = XcosMessages.FILL_COLOR;
	public static final String SMALL_ICON = "";
	public static final int MNEMONIC_KEY = 0;
	public static final int ACCELERATOR_KEY = 0;
    
    /**
     * @param scilabGraph graph
     */
    public FilledColorAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }

    /**
     * @param scilabGraph graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
	return createMenu(scilabGraph, FilledColorAction.class);
    }

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
	XcosDiagram graph = (XcosDiagram) getGraph(null);
	Object[] selectedCells = graph.getSelectionCells();
	
	//if no cells are selected : Do nothing
	if (selectedCells.length == 0) { return; }

	Color newColor = JColorChooser.showDialog(getGraph(null).getAsComponent(), NAME, null);

	if (newColor != null) {
	    graph.setCellStyles(mxConstants.STYLE_FILLCOLOR, mxUtils.hexString(newColor));
    	}
    }
}
