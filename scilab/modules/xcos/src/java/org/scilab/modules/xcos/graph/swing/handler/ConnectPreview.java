/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.swing.handler;

import java.awt.event.MouseEvent;

import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.link.BasicLink;

import com.mxgraph.model.mxICell;

/**
 * Create a split on stop if applicable
 */
public class ConnectPreview extends com.mxgraph.swing.handler.mxConnectPreview {

    /**
     * Default constructor
     *
     * @param graphComponent
     *            the default graph component
     */
    public ConnectPreview(GraphComponent graphComponent) {
        super(graphComponent);
    }

    /**
     * Stop the current link creation
     *
     * @param commit
     *            true if any modification should be performed
     * @param e
     *            the mouse event
     * @return the cell
     */
    @Override
    public Object stop(boolean commit, MouseEvent e) {
        if (commit && previewState != null && sourceState != null
                && sourceState.getCell() instanceof BasicLink) {
            final XcosDiagram graph = (XcosDiagram) graphComponent.getGraph();
            final BasicLink lnk = (BasicLink) sourceState.getCell();

            final SplitBlock split = graph.addSplitEdge(startPoint, lnk);
            final mxICell cell = (mxICell) previewState.getCell();

            cell.setTerminal(split.getOut2(), true);

            return super.stop(commit, e);
        } else {
            return super.stop(commit, e);
        }
    }
}
