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

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;

/**
 * Constraint which specify how many blocks must be present on a graph to enable
 * the action.
 */
public final class MinimalNumberOfCellsConstraint extends ActionConstraint {
	private final int minimalCount;
	private int numberOfCells;

	/**
	 * Default constructor
	 * @param minimalCount the minimal number of cells for the action to be enabled
	 */
	public MinimalNumberOfCellsConstraint(int minimalCount) {
		numberOfCells = 0;
		
		this.minimalCount = minimalCount;
	}

	/**
	 * Install this constraint on a graph for the specific action.
	 * @param action the constrained action
	 * @param scilabGraph where to install constraint 
	 */
	public void install(final DefaultAction action, ScilabGraph scilabGraph) {
		super.install(action, scilabGraph);
		scilabGraph.addListener(mxEvent.CELLS_ADDED, this);
		scilabGraph.addListener(mxEvent.CELLS_REMOVED, this);
		scilabGraph.addListener(mxEvent.ROOT, this);
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
		final String name = evt.getName();
		
		if (name.equals(mxEvent.ROOT)) {
			/*
			 * After diagram loading
			 */
			numberOfCells = ((mxCell) ((ScilabGraph) sender).getModel()
					.getRoot()).getChildCount();
			
		} else if (name.equals(mxEvent.CELLS_ADDED) || name.equals(mxEvent.CELLS_REMOVED)) {
			/*
			 * After any an add/remove operation
			 */
			Object[] cells = (Object[]) evt.getProperty("cells");
			
			if (evt.getName().compareTo(mxEvent.CELLS_ADDED) == 0) {
				numberOfCells += cells.length;
			} else {
				numberOfCells -= cells.length;
			}
		}
		
		setEnabled(numberOfCells >= minimalCount);
	}
}
