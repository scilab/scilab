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

package org.scilab.modules.xcos.graph.swing;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.handler.ConnectionHandler;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.handler.mxConnectionHandler;

/**
 * Implement a specific {@link mxGraphComponent} for an Xcos diagram.
 */
public class GraphComponent extends ScilabComponent {

	/**
	 * Default constructor.
	 * 
	 * @param graph the graph
	 */
	public GraphComponent(XcosDiagram graph) {
		super(graph);
	}
	
	/**
	 * @return a new {@link ConnectionHandler} instance
	 * @see com.mxgraph.swing.mxGraphComponent#createConnectionHandler()
	 */
	@Override
	protected mxConnectionHandler createConnectionHandler() {
		return new ConnectionHandler(this);
	}
}
