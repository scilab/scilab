/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graph.actions.base;

import org.scilab.modules.graph.ScilabGraph;

import com.mxgraph.model.mxGraphModel;
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

        // add the root and the default parent to the minimal count
        this.minimalCount = minimalCount + 2;
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
        final mxGraphModel model = (mxGraphModel) ((ScilabGraph) sender).getModel();
        numberOfCells = model.getCells().size();

        setEnabled(numberOfCells >= minimalCount);
    }
}
