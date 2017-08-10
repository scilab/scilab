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
 * Common class for selection dependent actions.
 *
 * Children of this class will be activated when there something selected. If
 * not, the action will be disabled.
 */
public abstract class VertexSelectionDependantAction extends DefaultAction {

    /**
     * Enable the selection if there is at least a vertex in the selection.
     */
    private final class VertexSelectionDependantConstraint extends ActionConstraint {

        /**
         * Default constructor
         */
        public VertexSelectionDependantConstraint() {
            super();
        }

        /**
         * @param action the action
         * @param scilabGraph the current graph
         * @see org.scilab.modules.graph.actions.base.ActionConstraint#install(org.scilab.modules.graph.actions.base.DefaultAction,
         *      org.scilab.modules.graph.ScilabGraph)
         */
        @Override
        public void install(DefaultAction action, ScilabGraph scilabGraph) {
            super.install(action, scilabGraph);

            scilabGraph.getSelectionModel().addListener(mxEvent.UNDO, this);
        }

        /**
         * @param sender the sender
         * @param evt the event
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
         */
        public void invoke(Object sender, mxEventObject evt) {
            mxGraphSelectionModel selection = (mxGraphSelectionModel) sender;
            Object[] cells = selection.getCells();

            boolean vertexFound = false;
            if (cells != null) {
                for (Object object : cells) {
                    if (object instanceof mxCell) {
                        mxCell cell = (mxCell) object;
                        vertexFound = cell.isVertex();
                    }

                    if (vertexFound) {
                        break;
                    }
                }

                setEnabled(vertexFound);
            }
        }

    }

    /**
     * Default constructor
     *
     * @param scilabGraph
     *            The associated graph
     */
    public VertexSelectionDependantAction(ScilabGraph scilabGraph) {
        super(scilabGraph);

        if (scilabGraph != null) {
            VertexSelectionDependantConstraint c = new VertexSelectionDependantConstraint();
            c.install(this, scilabGraph);
        }
    }
}
