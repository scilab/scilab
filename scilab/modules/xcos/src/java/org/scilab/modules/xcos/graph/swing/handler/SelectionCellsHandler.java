/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
