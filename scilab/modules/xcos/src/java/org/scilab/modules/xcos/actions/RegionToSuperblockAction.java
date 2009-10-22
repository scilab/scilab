/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;

public class RegionToSuperblockAction extends DefaultAction {

	private RegionToSuperblockAction(ScilabGraph scilabGraph) {
		super(XcosMessages.REGION_TO_SUPERBLOCK, scilabGraph);
	}

	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.REGION_TO_SUPERBLOCK, null, new RegionToSuperblockAction(scilabGraph), null);
	}

	public void doAction() {
	    XcosDiagram graph = (XcosDiagram) getGraph(null);
	    graph.getModel().beginUpdate();
	    XcosDiagram superBlock = new XcosDiagram();

	    double minX = Double.MAX_VALUE;
	    double minY = Double.MAX_VALUE;
	    double maxX = Double.MIN_VALUE;
	    double maxY = Double.MIN_VALUE;
	    
	    for (int i = 0 ; i < graph.getSelectionCells().length ; ++i) {
		mxCell current = (mxCell) graph.getSelectionCells()[i];
		minX = Math.min(minX, current.getGeometry().getX());
		minY = Math.min(minY, current.getGeometry().getY());
		maxX = Math.max(maxX, current.getGeometry().getX());
		maxY = Math.max(maxY, current.getGeometry().getY());
	    }
	    
	    superBlock.getModel().beginUpdate();
	    superBlock.addCells(graph.getSelectionCells());
	    superBlock.getModel().endUpdate();

	    
	    BasicBlock insertMe = BasicBlock.createBlock("SUPER_f");
	    insertMe.setStyle("SUPER_f");
	    insertMe.getGeometry().setX((maxX + minX) / 2.0);
	    insertMe.getGeometry().setY((maxY + minY) / 2.0);
	    insertMe.setRealParameters(BlockWriter.convertDiagramToMList(superBlock));
	    graph.clearSelection();
	    graph.addCell(insertMe);
	    graph.setSelectionCell(insertMe);
	    graph.refresh();
	    graph.getModel().endUpdate();
	    
	}

}
