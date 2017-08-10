/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
