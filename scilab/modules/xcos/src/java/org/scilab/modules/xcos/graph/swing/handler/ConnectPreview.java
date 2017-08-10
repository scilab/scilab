/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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
