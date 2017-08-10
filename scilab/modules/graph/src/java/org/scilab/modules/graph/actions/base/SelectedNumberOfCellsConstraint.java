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

import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxGraphSelectionModel;

/**
 * Constraint which specify how many blocks must be selected on the block to
 * enable the action.
 */
public class SelectedNumberOfCellsConstraint extends ActionConstraint {
    private final int numberOfSelectedCells;

    /**
     * Default constructor
     *
     * @param numberOfSelectedCells
     *            The number of selected block needed to enable the action.
     */
    public SelectedNumberOfCellsConstraint(int numberOfSelectedCells) {
        this.numberOfSelectedCells = numberOfSelectedCells;
    }

    /**
     * Install
     * @param action the action
     * @param scilabGraph the graph
     * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
     *      org.scilab.modules.graph.ScilabGraph)
     */
    @Override
    public void install(DefaultAction action, ScilabGraph scilabGraph) {
        super.install(action, scilabGraph);

        scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
    }

    /**
     * Update the enable state
     * @param sender The sender of the event
     * @param evt the event
     * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
     */
    public void invoke(Object sender, mxEventObject evt) {
        mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
        Object[] cells = selection.getCells();
        setEnabled((cells != null) && (cells.length >= numberOfSelectedCells));
    }

}
