/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.event.MouseEvent;

import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.link.BasicLink;

import com.mxgraph.model.mxICell;
import com.mxgraph.view.mxCellState;
import com.mxgraph.view.mxGraph;


/**
 * Implement a workaround for a jgraphx bug.
 * 
 * @see <a href="http://www.jgraph.org/bugzilla/show_bug.cgi?id=47">http://www.jgraph.org/bugzilla/show_bug.cgi?id=47</a>
 */
public class ConnectPreview extends com.mxgraph.swing.handler.mxConnectPreview {
	
	/**
	 * Default constructor
	 * 
	 * @param graphComponent the default graph component
	 */
	public ConnectPreview(GraphComponent graphComponent) {
		super(graphComponent);
	}
	
	/**
	 * Create a new edge on the graph.
	 * @param startState the startState
	 * @param style the current style
	 * @return the new cell
	 * @see com.mxgraph.swing.handler.mxConnectPreview#createCell(com.mxgraph.view.mxCellState, java.lang.String)
	 */
	@Override
	protected Object createCell(mxCellState startState, String style) {
		final mxGraph graph = graphComponent.getGraph();
		final mxICell cell = ((mxICell) graph.createEdge(null, null, "",
				startState.getCell(), null, style));
		((mxICell) startState.getCell()).insertEdge(cell, true);

		return cell;
	}
	
	/**
	 * Stop the current link creation
	 * @param commit true if any modification should be performed 
	 * @param e the mouse event
	 */
	@Override
	public Object stop(boolean commit, MouseEvent e) {
		if (commit && previewState != null && sourceState != null && sourceState.getCell() instanceof BasicLink) {
			final XcosDiagram graph = (XcosDiagram) graphComponent.getGraph();
			final BasicLink lnk = (BasicLink) sourceState.getCell();
			
			final SplitBlock split = graph.addSplitEdge(startPoint, lnk);
			final mxICell cell = (mxICell) previewState.getCell();
			
			cell.setTerminal(split.getOut2(), true);

			return super.stop(commit, e);
		} else {
			return super.stop(commit, e);
		}
	}
}
