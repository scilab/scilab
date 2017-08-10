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

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.view.mxGraphSelectionModel;

/**
 * Update a menu when a specific kind of cell is selected.
 */
public class SpecificCellSelectedConstraint extends ActionConstraint {
    private final Class < ? extends mxCell > kind;

    /**
     * Default constructor
     * @param kind The kind of cell which enable/disbale the action.
     */
    public SpecificCellSelectedConstraint(Class < ? extends mxCell > kind) {
        this.kind = kind;
    }

    /**
     * Install
     *
     * @param action
     *            the current action
     * @param scilabGraph
     *            tthe scilab graph
     * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
     *      org.scilab.modules.graph.ScilabGraph)
     */
    @Override
    public void install(DefaultAction action, ScilabGraph scilabGraph) {
        super.install(action, scilabGraph);

        scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
    }

    /**
     * @param sender the sender object
     * @param evt the event
     * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
     */
    public void invoke(Object sender, mxEventObject evt) {
        mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
        Object[] cells = selection.getCells();

        for (Object object : cells) {
            if (kind.isInstance(object)) {
                setEnabled(true);
                return;
            }
        }

        setEnabled(false);
    }

}
