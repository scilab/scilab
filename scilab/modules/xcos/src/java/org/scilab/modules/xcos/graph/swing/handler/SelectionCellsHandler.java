/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.graph.swing.handler;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.handler.mxCellHandler;
import com.mxgraph.swing.handler.mxSelectionCellsHandler;

/**
 * Add a clearCellHandler method
 */
public class SelectionCellsHandler extends mxSelectionCellsHandler {

    /**
     * Default constructor
     *
     * @param graphComponent
     *            the component
     */
    public SelectionCellsHandler(mxGraphComponent graphComponent) {
        super(graphComponent);
    }

    /**
     * Clear the corresponding cell handler to force a cache miss
     *
     * @param cell
     *            the cell
     * @return the removed handler
     */
    public mxCellHandler clearCellHandler(Object cell) {
        if (handlers == null) {
            return null;
        }

        Object removed = handlers.remove(cell);
        return (mxCellHandler) removed;
    }
}
