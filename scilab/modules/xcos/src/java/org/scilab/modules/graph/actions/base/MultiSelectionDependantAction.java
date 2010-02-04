/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.actions.base;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxEventSource.mxIEventListener;

/**
 * Common class for multi-selection dependent actions.
 * 
 * Children of this class will be activated when there something selected. If
 * not, the action will be disabled.
 */
public abstract class MultiSelectionDependantAction extends DefaultAction {

	/**
	 * Implement the listener to install on the graph
	 */
	private class MultiSelectionChangeListener implements mxIEventListener {
		private ScilabGraph scilabGraph;

		/**
		 * Default constructor
		 * 
		 * @param scilabGraph
		 *            the associated graph
		 */
		public MultiSelectionChangeListener(ScilabGraph scilabGraph) {
			this.scilabGraph = scilabGraph;
		}

		/**
		 * Enable or disable the action.
		 * 
		 * @param sender
		 *            the sender
		 * @param evt
		 *            the event
		 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
		 *      com.mxgraph.util.mxEventObject)
		 */
		public void invoke(Object sender, mxEventObject evt) {
			Object[] cells = scilabGraph.getSelectionCells();
			setEnabled((cells != null) && (cells.length > 1));
		}

	}

	/**
	 * Default constructor
	 * 
	 * @param scilabGraph
	 *            the associated graph
	 */
	public MultiSelectionDependantAction(ScilabGraph scilabGraph) {
		super(scilabGraph);

		if (scilabGraph != null) {
			scilabGraph.getSelectionModel().addListener(mxEvent.UNDO,
					new MultiSelectionChangeListener(scilabGraph));
		}
	}
}
